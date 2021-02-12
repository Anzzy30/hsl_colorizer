#include <vector>
#include <iostream>
#include <cmath>
namespace hsl_colorizer {

    template <typename Scalar>
    class vec3{
    public:
        Scalar x=0,y=0,z=0;

//        float operator*(const vec3 <Scalar> &in) const{
//            return x*in.x+y*in.y+z*in.z;
//        }

        vec3 operator+(const vec3<Scalar> &in) const{
            return {x+in.x,y+in.y,z+in.z};
        }

        vec3 operator*(const Scalar v) const{
            return {x*v,y*v,z*v};
        }

    };

    struct hsl{
        float h=0,s=0,l=0;
    };
    struct rgb{
        float r=0,g=0,b=0;
    };

    float normalize(float v,float min, float max){
        return (v-min)/(max-min);
    }


    float hue2rgb(float p,float q,float t){
        if(t < 0.f) t += 1.f;
        if(t > 1.f) t -= 1.f;
        if(t < 1.f/6.f) return p + (q - p) * 6.f * t;
        if(t < 1.f/2.f) return q;
        if(t < 2.f/3.f) return p + (q - p) * (2.f/3.f - t) * 6.f;
        return p;
    }

    hsl rgb_to_hsl(const rgb &rgb){
        float max = std::max(rgb.r, std::max(rgb.g, rgb.b)), min = std::min(rgb.r, std::min(rgb.g, rgb.b));
        hsl hsl; hsl.l = (max + min) / 2;

        if(max != min){

            float d = max - min;
            hsl.s = hsl.l > 0.5 ? d / (2.f - max - min) : d / (max + min);
            if(max == rgb.r) hsl.h = (rgb.g - rgb.b) / d + (rgb.g < rgb.b ? 6.f : 0.f);
            if(max == rgb.g) hsl.h = (rgb.b - rgb.r) / d + 2.f;
            if(max == rgb.b) hsl.h = (rgb.r - rgb.g) / d + 4.f;
            hsl.h /= 6.f;
        }

        return hsl;
    }

    rgb hsl_to_rgb(const hsl &hsl){
        rgb rgb;
        if(hsl.s!=0){
            float q = hsl.l < 0.5 ? hsl.l * (1 + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
            float p = 2 * hsl.l - q;
            rgb.r = hue2rgb(p, q, hsl.h + 1.f/3.f);
            rgb.g = hue2rgb(p, q, hsl.h);
            rgb.b = hue2rgb(p, q, hsl.h - 1.f/3.f);

        }else{
            rgb.r = rgb.b = rgb.g = hsl.l;
        }

        return rgb;
    }


    float interpolate( float val, float y0, float x0, float y1, float x1 ) {
        return (val-x0)*(y1-y0)/(x1-x0) + y0;
    }

    float jet_base( float val ) {
        if ( val <= 0.125 ) return 0;
        else if ( val <= 0.375 ) return interpolate( val, 0.0, 0.125, 1.0, 0.375 );
        else if ( val <= 0.625 ) return 1.0;
        else if ( val <= 0.875 ) return interpolate( val, 1.0, 0.625, 0.0, 0.875 );
        else return 0.;
    }
    rgb jet(float v){
        rgb color;
        color.r = jet_base(v-0.25f);
        color.g = jet_base(v);
        color.b = jet_base(v+0.25f);
        return color;
    }
    rgb jethsl(float v,float s=0.95f,float l= 0.75f){
        rgb rgb_color = jet(v);
        hsl hsl_color = rgb_to_hsl(rgb_color);
        hsl_color.s=s;hsl_color.l=l;
        return hsl_to_rgb(hsl_color);
    }


//     operator* in vec3
//    0.8, 0.5, 0.4		0.2, 0.4, 0.2	2.0, 1.0, 1.0	0.00, 0.25, 0.25


    rgb iquilezles_fn(float v, vec3<float> a, vec3<float> b, vec3<float> c, vec3<float> d){

        rgb color;
        color.r = a.x + b.x*std::cos( 6.28318f*(c.x*v+d.x));
        color.g = a.y + b.y*std::cos( 6.28318f*(c.y*v+d.y));
        color.b = a.z + b.z*std::cos( 6.28318f*(c.z*v+d.z));
        return color;
    }

    rgb morgantrouvesaboxd(float v){
        return iquilezles_fn(v,{0.8f, 0.5f, 0.4f},{.2f, 0.4f, 0.2f},{2.0f, 1.0f, 1.0f},{0.00f, 0.25f, 0.25f});
    }

    rgb hsl_morgantrouvesaboxd(float v,float s=0.95f,float l= 0.75f){
       rgb rgb_color = iquilezles_fn(v,{0.8f, 0.5f, 0.4f},{.2f, 0.4f, 0.2f},{2.0f, 1.0f, 1.0f},{0.00f, 0.25f, 0.25f});
       hsl hsl_color = rgb_to_hsl(rgb_color);
       hsl_color.s=s;hsl_color.l=l;
       return hsl_to_rgb(hsl_color);

    }

    rgb full_lerp_hsl(const float data, const float s, const float l){
        return hsl_to_rgb({data,s,l});
    }









}

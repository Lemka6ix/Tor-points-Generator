#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <sstream>

struct point3d
{  
    double x; 
    double y;   
    double z; 

    point3d(double x = 0.0, double y = 0.0, double z = 0.0) : 
        x(x), y(y), z(z) {}
        
        void print() const {
         std::cout << "(" << x << ", " << y << ", " << z << ")\n";
    }

    double getBackX() const { return x; }
    double getBackY() const { return y; }
    double getBackZ() const { return z; }
};


class Figure
{
private:
    double R;             // Большой радиус тора (расстояние от центра до оси вращения)
    double r;             // Малый радиус тора (радиус образующей окружности)
    int K;                // Количество точек для генерации
    point3d center;       // Центр тора
    double tilt_angle_x;  // Угол наклона вокруг оси X (в радианах)
    double tilt_angle_y;  // Угол наклона вокруг оси Y (в радианах)
    double tilt_angle_z;  // Угол наклона вокруг оси Z (в радианах)

   
    static std::mt19937& getGenerator() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }


    point3d rotate_point(const point3d& point) const {
        double x = point.x;
        double y = point.y;
        double z = point.z;
        
        if (tilt_angle_z != 0) {
            double cos_z = cos(tilt_angle_z);
            double sin_z = sin(tilt_angle_z);
            double new_x = x * cos_z - y * sin_z;
            double new_y = x * sin_z + y * cos_z;
            x = new_x;
            y = new_y;
        }
        
        if (tilt_angle_y != 0) {
            double cos_y = cos(tilt_angle_y);
            double sin_y = sin(tilt_angle_y);
            double new_x = x * cos_y + z * sin_y;
            double new_z = -x * sin_y + z * cos_y;
            x = new_x;
            z = new_z;
        }
        
        if (tilt_angle_x != 0) {
            double cos_x = cos(tilt_angle_x);
            double sin_x = sin(tilt_angle_x);
            double new_y = y * cos_x - z * sin_x;
            double new_z = y * sin_x + z * cos_x;
            y = new_y;
            z = new_z;
        }
        
        return point3d(x, y, z);
    }

    point3d translate_point(const point3d& point) const {
        return point3d(point.x + center.x, 
                      point.y + center.y, 
                      point.z + center.z);
    }

public:
    std::vector<point3d> points_arr;

    Figure(double R = 3.0, double r = 1.0, 
           double center_x = 0.0, double center_y = 0.0, double center_z = 0.0,
           double tilt_x = 0.0, double tilt_y = 0.0, double tilt_z = 0.0) : 
        R(R), r(r), center(center_x, center_y, center_z),
        tilt_angle_x(tilt_x * M_PI / 180.0),  // Перевод градусов в радианы
        tilt_angle_y(tilt_y * M_PI / 180.0),
        tilt_angle_z(tilt_z * M_PI / 180.0) {}


    point3d get_torus_point(double u, double v) const {

        double x = (R + r * cos(v)) * cos(u);
        double y = (R + r * cos(v)) * sin(u);
        double z = r * sin(v);
        
        point3d point(x, y, z);
        
        point = rotate_point(point);
        point = translate_point(point);
        
        return point;
    }


    point3d rnd()
    {
        std::uniform_real_distribution<double> uDist(0, 2 * M_PI);
        std::uniform_real_distribution<double> vDist(0, M_PI);
        std::uniform_real_distribution<double> radiusDist(0, r);
        
        double u = uDist(getGenerator());
        double v = vDist(getGenerator());
        double random_r = radiusDist(getGenerator());
        
        double x = (R + random_r * cos(v)) * cos(u);
        double y = (R + random_r * cos(v)) * sin(u);
        double z = random_r * sin(v);
        
        point3d point(x, y, z);
        
        point = rotate_point(point);
        point = translate_point(point);
        
        return point;
    }

    void filling_K(int K)
    {
        points_arr.clear(); // Очищаем массив перед заполнением
        for (int i = 0; i < K; i++)
        {
            points_arr.push_back(rnd());
        }
    }
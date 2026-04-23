#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <sstream>

/**
 * @file 1.cpp
 * @brief Основной файл программы для генерации точек на торе
 * @author Климентий
 * @version 1.0
 * @date 04.2026
 */

/**
 * @brief Структура для представления точки в трехмерном пространстве
 */
struct point3d
{  
    double x;  ///< Координата X 
    double y;  ///< Координата y 
    double z;  ///< Координата z


    /**
     * @brief Конструктор точки
     * @param x Координата X (по умолчанию 0.0)
     * @param y Координата Y (по умолчанию 0.0)
     * @param z Координата Z (по умолчанию 0.0)
     */
    point3d(double x = 0.0, double y = 0.0, double z = 0.0) : 
        x(x), y(y), z(z) {}
        
        /**
        * @brief Вывод координат точки на экран
        */
        void print() const {
         std::cout << "(" << x << ", " << y << ", " << z << ")\n";
    }

    /**
     * @brief Получение координаты X
     * @return Координата X точки
     */
    double getBackX() const { return x; }
    
    /**
     * @brief Получение координаты Y
     * @return Координата Y точки
     */
    double getBackY() const { return y; }

    /**
     * @brief Получение координаты Z
     * @return Координата Z точки
     */
    double getBackZ() const { return z; }
};

/**
 * @brief Класс для работы с тором и генерации точек
 */
class Figure
{
private:
    double R;             ///< Большой радиус тора (расстояние от центра до оси вращения)
    double r;             ///< Малый радиус тора (радиус образующей окружности)
    int K;                ///< Количество точек для генерации
    point3d center;       ///< Центр тора
    double tilt_angle_x;  ///< Угол наклона вокруг оси X (в радианах)
    double tilt_angle_y;  ///< Угол наклона вокруг оси Y (в радианах)
    double tilt_angle_z;  ///< Угол наклона вокруг оси Z (в радианах)

    
    /**
     * @brief Получение генератора случайных чисел
     * @return Ссылка на генератор случайных чисел
     */
    static std::mt19937& getGenerator() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }


    /**
     * @brief Поворот точки вокруг осей координат
     * @param point Исходная точка
     * @return Повернутая точка
     */
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


    /**
     * @brief Поворот точки вокруг осей координат
     * @param point Исходная точка
     * @return Повернутая точка
     */
    point3d translate_point(const point3d& point) const {
        return point3d(point.x + center.x, 
                      point.y + center.y, 
                      point.z + center.z);
    }

public:
    std::vector<point3d> points_arr;   ///< Массив точек тора


    /**
     * @brief Конструктор класса Figure
     * @param R Большой радиус тора (по умолчанию 3.0)
     * @param r Малый радиус тора (по умолчанию 1.0)
     * @param center_x Координата X центра тора (по умолчанию 0.0)
     * @param center_y Координата Y центра тора (по умолчанию 0.0)
     * @param center_z Координата Z центра тора (по умолчанию 0.0)
     * @param tilt_x Угол наклона вокруг оси X в градусах (по умолчанию 0.0)
     * @param tilt_y Угол наклона вокруг оси Y в градусах (по умолчанию 0.0)
     * @param tilt_z Угол наклона вокруг оси Z в градусах (по умолчанию 0.0)
     */
    Figure(double R = 3.0, double r = 1.0, 
           double center_x = 0.0, double center_y = 0.0, double center_z = 0.0,
           double tilt_x = 0.0, double tilt_y = 0.0, double tilt_z = 0.0) : 
        R(R), r(r), center(center_x, center_y, center_z),
        tilt_angle_x(tilt_x * M_PI / 180.0),  // Перевод градусов в радианы
        tilt_angle_y(tilt_y * M_PI / 180.0),
        tilt_angle_z(tilt_z * M_PI / 180.0) {}

    
    
    /**
     * @brief Вычисление координат точки на торе по параметрическим уравнениям
     * @param u Угол большого круга (от 0 до 2π)
     * @param v Угол малого круга (от 0 до π)
     * @return Точка на поверхности тора с учетом центра и наклона
     */
    point3d get_torus_point(double u, double v) const {

        double x = (R + r * cos(v)) * cos(u);
        double y = (R + r * cos(v)) * sin(u);
        double z = r * sin(v);
        
        point3d point(x, y, z);
        
        point = rotate_point(point);
        point = translate_point(point);
        
        return point;
    }



    /**
     * @brief Генерация случайной точки внутри верхней половины тора
     * @return Случайная точка внутри тора с учетом центра и наклона
     */
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


    /**
     * @brief Заполнение массива K случайными точками
     * @param K Количество точек для генерации
     */
    void filling_K(int K)
    {
        points_arr.clear(); // Очищаем массив перед заполнением
        for (int i = 0; i < K; i++)
        {
            points_arr.push_back(rnd());
        }
    }


    /**
     * @brief Получение точки по индексу из массива
     * @param i Индекс точки в массиве
     * @return Точка с указанным индексом или точка по умолчанию при ошибке
     */
    point3d i_point_position(int i)
    {
        if (i < 0 || i >= points_arr.size()) {
            std::cout << "Incorrect index\n";
            return point3d();
        }
        return points_arr[i];
    }
    

    /**
     * @brief Добавление пользовательской точки в массив
     * @param x Координата X новой точки
     * @param y Координата Y новой точки
     * @param z Координата Z новой точки
     */
    void new_user_point(double x, double y, double z)
    {
        points_arr.push_back(point3d(x, y, z));
    }


    /**
     * @brief Получение текущих углов наклона тора (в градусах)
     * @return Вектор углов наклона [x, y, z]
     */
    std::vector<double> get_tilt_angles() const {
        return {tilt_angle_x * 180.0 / M_PI, 
                tilt_angle_y * 180.0 / M_PI, 
                tilt_angle_z * 180.0 / M_PI};
    }



    /**
     * @brief Запись всех точек в файл points.txt
     * @details Формат файла: каждая строка содержит три координаты, разделенные табуляцией
     */
    void write_data_to_file() {
        std::ofstream out;
        out.open("points.txt");
        if (out.is_open())
        {
            for (int i = 0; i < points_arr.size(); i++)
            {
                out << points_arr[i].x << "\t" << points_arr[i].y << "\t" << points_arr[i].z << "\n";
            }
        }
        out.close();
    }


    /**
     * @brief Запуск визуализации точек с помощью Python скрипта
     * @details Вызывает скрипт visualize.py для отображения тора и точек
     */
    void run_visualization() {
        std::stringstream command;
        command << "python visualize.py";
        
        std::cout << "Running: " << command.str() << std::endl;
        int result = system(command.str().c_str());
        
        if (result == 0) {
            std::cout << "Visualization completed" << std::endl;
        } else {
            std::cout << "Visualization error" << std::endl;
        }
    }


    /**
     * @brief Запись параметров тора в файл setting.dat
     * @param K Количество точек
     * @param R Большой радиус тора
     * @param r Малый радиус тора
     */
    void write_parameters_to_file(int K, double R, double r) {
        std::ofstream out;
        out.open("setting.dat");
        if (out.is_open())
        {
            out << "K: " << K << "\n" 
                << "R: " << R << "\n" 
                << "r: " << r << "\n"
                << "Center: (" << center.x << ", " << center.y << ", " << center.z << ")\n"
                << "Tilt angles (degrees): X=" << tilt_angle_x * 180.0 / M_PI 
                << ", Y=" << tilt_angle_y * 180.0 / M_PI 
                << ", Z=" << tilt_angle_z * 180.0 / M_PI << "\n";      
        }
        out.close();
    }


    /**
     * @brief Вывод всех точек массива на экран
     */
    void printPoints() {
        std::cout << "Points in vector (" << points_arr.size() << " points):\n";
        for (auto point : points_arr) {
            point.print();
        }
    }
};


/**
 * @brief Функция отображения меню и получения выбора пользователя
 * @param choise Текущий выбор пользователя
 * @return Выбор пользователя
 */
int menu_func(int choise)
{
    std::cout << "Menu:\n" 
            << "1. i point coords\n"                      // координата точки
            << "2. Add new point\n"                       // добавить точку
            << "3. Save data in points.txt\n"             // сохранить точки в файл .txt
            << "4. Save parameters in setting.dat\n"      // сохранить параметры в файл .bat
            << "5. Visualization (matplotlib)\n"          // визуализация
            << "0. Exit\n"
            << "Enter your choise: ";

    std::cin >> choise;
    return choise;
}


/**
 * @brief Главная функция программы
 * @return Код завершения программы
 */
int main()
{
    int K;        ///< Количество точек        
    double R;     ///< Большой радиус тора
    double r;     ///< Малый радиус тора
    int choise;   ///< Выбор пользователя в меню
    int i;        ///< Индекс точки
    double x, y, z;      ///< Координаты точки
    double center_x, center_y, center_z;     ///< Координаты центра
    double tilt_x, tilt_y, tilt_z;           ///< Углы наклона

    std::cout << "Enter K: ";
    std::cin >> K;
    std::cout << "Enter R: ";
    std::cin >> R;
    std::cout << "Enter r: ";
    std::cin >> r;

    std::cout << "Enter torus center (x y z): ";
    std::cin >> center_x >> center_y >> center_z;

    std::cout << "Enter tilt angles in degrees (x y z): ";
    std::cin >> tilt_x >> tilt_y >> tilt_z;

    Figure fig(R, r, center_x, center_y, center_z, tilt_x, tilt_y, tilt_z);
    fig.filling_K(K);

    while (choise != 0) {
        choise = menu_func(choise);

        switch (choise)
        {
            case 1: {
                std::cout << "i point position. Enter index i: ";
                std::cin >> i;
                point3d i_point = fig.i_point_position(i);
                i_point.print();
                std::cout << "\n";
                break;
            }
            case 2: {
                std::cout << "Enter coords of new point (x y z): ";
                std::cin >> x >> y >> z;
                fig.new_user_point(x, y, z);
                std::cout << "Point added.\n\n";
                break;
            }
            case 3: {
                fig.write_data_to_file();
                std::cout << "Data saved to points.txt\n\n";
                break;
            }
            case 4: {
                fig.write_parameters_to_file(K, R, r);
                std::cout << "Parameters saved to setting.dat\n\n";
                break;
            }
            case 5: {
                fig.run_visualization();
                std::cout << "\n";
                break;
            }
            case 0: {
                return 0;
            }
            default: {
                std::cout << "Incorrect choise\n";
                break;
            }
        }
    }

    return 0;
}
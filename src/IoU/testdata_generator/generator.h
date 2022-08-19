#include <iostream>
#include <vector>
#include <fstream>
#include <opencv2/core/core.hpp>
// Drawing shapes
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace gen{
    struct SET{
        int x;
        int y;
        int w;
        int h;
    };
    class GENERATOR{
        public:
        GENERATOR(double x, double y, double w, double h, int width, int height, int num, int start, int end);
        ~GENERATOR();
        inline std::vector<std::vector<int>> getBBoxes(){
            return BBoxes;
        }
        inline cv::Mat getImageOutput(){
            if(!output.empty()){
                return output;
                }
        }
        void saveCoordinates(std::string filename);
        void saveCoordinatesVector(std::string filename, std::vector<std::vector<int>> data, int index_1, int index_2);
        inline void writeImages(std::string filename, int index_1, int index_2){
            cv::Mat image = getImageOutput();
            std::string foutput = filename + std::to_string(index_1) + "_"+std::to_string(index_2) + ".png";
            //std::cout<< foutput<<std::endl;
            cv::imwrite(foutput, image);
        }
        private:

        std::vector<std::vector<int>> BBoxes;
        cv::Mat output;
        double xx, yy, ww, hh;
        int width, height;
        SET COORDs;

        cv::Mat createMatrix(float value){
            return cv::Mat::zeros(cv::Size(this->width,this->height), CV_8UC1); //cvScalar
            }
        int random(int low, int high)
        {
            return low + rand() % (high - low + 1);
        }
        void setCoordinates(int num, int start, int end);
        void create_rectangle();
        void scaleCoordinates(double x, double y, double w, double h, int width, int height);

    };
    inline void convertStringToFloat(std::string &line, std::vector<double>&vect ){

        std::stringstream converter(line);
        double i;
        while(converter>>i){
            vect.push_back(i);
            if(converter.peek() ==','){
                converter.ignore();

            }
        }
    }
}
#include "generator.h"
#include <cassert>
namespace gen{
GENERATOR::GENERATOR(double x, double y, double w, double h, int width, int height, int num, int start, int end){
    this->xx=x;
    this->yy =y;
    this->ww =w;
    this->hh =h;
    this->width = width;
    this->height = height;
    scaleCoordinates( x,  y,  w,  h, width, height);
    setCoordinates(num, start, end);
    create_rectangle();
}
GENERATOR::~GENERATOR(){
}

void GENERATOR::setCoordinates( int num, int start, int end){
        std::vector<int> coordinates;
        coordinates.push_back(COORDs.x);
        coordinates.push_back(COORDs.y);
        coordinates.push_back(COORDs.w);
        coordinates.push_back(COORDs.h);
        BBoxes.push_back(coordinates);
        coordinates.clear();
        for(int i = 0; i<num;i++){
            int ra = random(start,end);
            COORDs.x+=ra;
            COORDs.y+=ra;
            COORDs.w+=ra;
            COORDs.h+=ra;
            coordinates.push_back(COORDs.x);
            coordinates.push_back(COORDs.y);
            coordinates.push_back(COORDs.w);
            coordinates.push_back(COORDs.h);
            BBoxes.push_back(coordinates);
            coordinates.clear();

        }
    }

void GENERATOR::create_rectangle(){
    assert(BBoxes.size()>0);
    cv::Mat image = createMatrix(0.);
    for(int i = 0; i<BBoxes.size();i++){
        cv::Point p1(BBoxes[i][0], BBoxes[i][1]);
        cv::Point p2(BBoxes[i][2], BBoxes[i][3]);
        cv::rectangle(image, p1, p2, cv::Scalar(255, 0, 0), 2, cv::LINE_8);
    }
    image.copyTo(output);

}

void GENERATOR::saveCoordinates(std::string filename){
    std::ofstream file(filename);
    file << 0<< " " <<this->xx << " " << this->yy << " " <<this->ww << " " << this->hh <<std::endl;
    file.close();

}
void GENERATOR::saveCoordinatesVector(std::string filename, std::vector<std::vector<int>> data, int index_1, int index_2){
    std::string foutput = filename + std::to_string(index_1) + "_" + std::to_string(index_2) +".txt";
    //std::cout << foutput<<std::endl;
    std::ofstream file(foutput);
    for(int i =0; i<data.size();i++){
        file << 0<< " " <<data[i][0] << " " << data[i][1] << " " <<data[i][2] << " " << data[i][3] <<std::endl;
    }
    file.close();
}
void GENERATOR::scaleCoordinates(double x, double y, double w, double h, int width, int high){
    int xx = (int) (x * width);
    int yy = (int) (y * high);
    int ww = (int) (w * width);
    int hh = (int) (h * high);

    int X = (int)(xx - (ww / 2));
    int Y = (int)(yy - (hh / 2));
    std::cout << "x -> "<< X << " y-> "<< Y << " width -> "<< ww<< " height -> " << hh<<std::endl;
    COORDs.x = X;
    COORDs.y = Y;
    COORDs.w = X+ww;
    COORDs.h = Y+hh;

}

}
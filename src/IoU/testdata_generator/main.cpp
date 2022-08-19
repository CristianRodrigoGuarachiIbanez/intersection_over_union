//#include <fstream>
#include <string>
#include <vector>
//#include <sstream>
#include <iostream>
#include "generator.h"


int main(){
   std::vector<double> coordinates;

   std::fstream file;

   file.open("./file.txt");
   if(file.is_open()){
        std::string line;
        double x,y,w,h;
        int k =0;
        while(std::getline(file,line)){
            //std::cout<< " line "<< line<<std::endl;
            gen::convertStringToFloat(line, coordinates);
            x = coordinates[1];
            y = coordinates[2];
            w = coordinates[3];
            h = coordinates[4];
            //TODO run function
            //std::cout<<" x -> "<< x << "y -> "<< y << " w -> "<< w<< "h ->" << h<< std::endl;
            coordinates.clear();
            int lim = 2;
            int numberBBoxes = 2;
            for (int i = 0;i<lim;i++){

                gen::GENERATOR gen( x,y,w,h, 416,416, numberBBoxes, 0,15);//228,248,18+228,22+248,2+x);

                std::vector<std::vector<int>> coord = gen.getBBoxes();
                std::string filename = "./images_3/image_"; // + std::to_string(k) + "_" + std::to_string(i);
                //gen.saveCoordinates(filename + ".txt");
                gen.saveCoordinatesVector(filename, coord, i, k);
                gen.writeImages(filename, i,k);

                //cv::imshow("Output", image);
                //cv::waitKey(0);
                // cv:imwrite(filename + ".png", image);

                std::cout<< " finished " <<std::endl;
            }
            k++;
        }
    }
    return 0;
}



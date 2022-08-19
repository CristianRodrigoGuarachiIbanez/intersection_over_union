#distutils: language = c++
from libcpp.vector cimport vector
from libcpp.string cimport string



# For cv::Mat usage
cdef extern from "opencv2/core/core.hpp":
  cdef int  CV_WINDOW_AUTOSIZE
  cdef int CV_8UC3
  cdef int CV_8UC1
  cdef int CV_32FC1
  cdef int CV_8U
  cdef int CV_32F

cdef extern from "opencv2/core/core.hpp" namespace "cv":
  cdef cppclass Mat:
    Mat() except +
    void create(int, int, int)
    void* data
    int rows
    int cols
    int channels()
    int depth()
    size_t elemSize()

# For Buffer usage
cdef extern from "Python.h":
    ctypedef struct PyObject
    object PyMemoryView_FromBuffer(Py_buffer *view)
    int PyBuffer_FillInfo(Py_buffer *view, PyObject *obj, void *buf, Py_ssize_t len, int readonly, int infoflags)
    enum:
        PyBUF_FULL_RO


cdef extern from "../generator.h" namespace "gen":
    cdef cppclass SET:
        int x;
        int y;
        int w;
        int h
    cdef cppclass GENERATOR:
        GENERATOR() except +
        GENERATOR(double x, double y, double w, double h, int width, int height, int num, int start, int end) except +
        inline vector[vector[int]] getBBoxes()
        inline Mat getImageOutput()
        void saveCoordinates(string filename);
        void saveCoordinatesVector(string filename, vector[vector[int]] data, int index_1, int index_2);
        void writeImages(string filename, Mat image, int index_1, int index_2)
        inline void writeImages(string filename, int index_1, int index_2)

        vector[vector[int]] BBoxes;
        Mat output;
        double xx, yy, ww, hh;
        int width, height;
        SET COORDs;
        Mat createMatrix(float value)
        int random(int low, int high)

        void setCoordinates(int num, int start, int end);
        void create_rectangle();
        void scaleCoordinates(double x, double y, double w, double h, int width, int height);

    cdef inline void convertStringToFloat(string &line, vector[double]&vect )

# distutils: language = c++
from libs.GENERATOR cimport *
from libcpp.vector cimport vector
from libcpp.string cimport string
from libc.stdio cimport FILE, fopen, feof, fgetc, getline,fclose
from libc.stdlib cimport malloc, free
from libcpp.cast cimport static_cast
from cython cimport boundscheck, wraparound, cdivision
from numpy import ndarray, asarray, dstack, uint8, float32, zeros

ctypedef unsigned char uchar
cdef class Generator:
    cdef:
        vector[double] _coordinates
        int _limit, _numBBox
        vector[vector[Mat]] _images
    def __cinit__(self, char * filename, char *fname, int limit, int numBBox):
        self._limit = limit
        self._numBBox = numBBox
        if (filename is None):
           raise ValueError("Could not find a path to the file which contains the table of distances")
        else:
           self.open_file(filename, fname)
           #self.convertToNumpyArray(self._images)


    @boundscheck(False)
    @wraparound(False)
    @cdivision(True)
    cdef void open_file(self, char * filename, const char * fname):
        cdef:
            FILE * content
            char * line = NULL
            size_t length = 0
            size_t nread
            double x,y,w,h
            int i=0, k=0
            GENERATOR * generator
            vector[vector[int]] coord
            Mat image
            vector[Mat] images
        content = fopen(filename, "r")
        if (content is NULL):
            print ("I/O Error: cannot open file {}".format( filename))
        else:
            nread = getline(&line, &length, content)
            k = 0
            while(nread !=-1 ):
                # print("content -> ",nread)
                convertStringToFloat(string(line), self._coordinates)
                # print("Floating Coordinates -> ",self._coordinates, line)
                x = self._coordinates[1]
                y = self._coordinates[2]
                w = self._coordinates[3]
                h = self._coordinates[4]
                self._coordinates.clear()
                images.clear()
                for i in range(self._limit):
                    generator = new GENERATOR(x,y,w,h, 416,416, self._numBBox, 0,15)
                    coord = generator.getBBoxes()
                    image = generator.getImageOutput()

                    images.push_back(image)

                    generator.saveCoordinatesVector(fname, coord, i, k)
                    generator.writeImages(fname, i, k)
                self._images.push_back(images)
                nread = getline(&line, &length, content)
                k+=1
            free(line)


    @boundscheck(False)
    @wraparound(False)
    cdef inline vector[vector[Mat]] getImages(self):
        if(self._images.size()!=0):
            return self._images

    @boundscheck(False)
    @wraparound(False)
    cdef uchar[:,:,:,:] convertToNumpyArray(self, vector[vector[Mat]] _images):
        #print("size", self._images.size())
        cdef:
            int i,j
            output = []
        for i in range(_images.size()):
            images =[]
            if(self._images[i].size()!=self._limit):
                print("the given limit of images and the actual one are not similar!")
            for j in range(self._limit):
                img = self.Mat2np(_images[i][j])
                #print(img.shape)
                images.append(img)
            output.append(images)
        return asarray(output, dtype=uint8)

    @boundscheck(False)
    @wraparound(False)
    @cdivision(True)
    cdef inline object Mat2np(self, Mat&m):
        # Create buffer to transfer data from m.data
        cdef Py_buffer buf_info

        # Define the size / len of data
        cdef size_t len = m.rows*m.cols*m.elemSize()  #m.channels()*sizeof(CV_8UC3)

        # Fill buffer
        PyBuffer_FillInfo(&buf_info, NULL, m.data, len, 1, PyBUF_FULL_RO)

        # Get Pyobject from buffer data
        Pydata  = PyMemoryView_FromBuffer(&buf_info)

        # Create ndarray with data
        #print("channels ->", m.channels(), m.depth(), CV_32F)
        # Create ndarray with data
        # the dimension of the output array is 2 if the image is grayscale
        if (m.channels()==2 ):
            shape_array = (m.rows, m.cols, m.channels())
        elif(m.channels()==3):
            shape_array = (m.rows, m.cols, m.channels())
        else:
            shape_array = (m.rows, m.cols)

        if m.depth() == CV_32F :
            array = ndarray(shape=shape_array, buffer=Pydata, order='c', dtype=float32)
        else :
            #8-bit image
            array = ndarray(shape=shape_array, buffer=Pydata, order='c', dtype=uint8)

        if m.channels() == 3:
            # BGR -> RGB
            array = dstack((array[...,2], array[...,1], array[...,0]))

        return asarray(array, dtype=uint8)

    def getGeneratedImages(self):
        return self.convertToNumpyArray(self._images)
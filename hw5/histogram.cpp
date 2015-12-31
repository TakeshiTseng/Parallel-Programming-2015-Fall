#include <cstdio>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <CL/cl.h>
using namespace std;

const char* source_code =
"__kernel void cl_histogram(__global const int* image_data, __global int* res) {"
"    int idx = (int)get_global_id(0);"
"    int width = 256;"
"    if(idx % 3 == 0) {"
"        atom_inc(&res[image_data[idx] + width * 0]);"
"    } else if(idx % 3 == 1) {"
"        atom_inc(&res[image_data[idx] + width * 1]);"
"    } else {"
"        atom_inc(&res[image_data[idx] + width * 2]);"
"    }"
"}";

cl_program load_program(cl_context context, const char* source) {
    cl_program program = clCreateProgramWithSource(context, 1, &source, 0, 0);
    if(program == 0) {
        return 0;
    }
    if(clBuildProgram(program, 0, 0, 0, 0, 0) != CL_SUCCESS) {
        return 0;
    }
    return program;
}

int main(int argc, char const *argv[])
{

    unsigned int* histogram_results;
    unsigned int i=0, a, input_size;
    unsigned int *image = new unsigned int[input_size];

    fstream inFile("input", ios_base::in);
    ofstream outFile("0356063.out", ios_base::out);

    inFile >> input_size;
    histogram_results = (unsigned int*)malloc(sizeof(int) * 256 * 3);
    memset(histogram_results, 0, sizeof(int) * 256 * 3);

    while( inFile >> a ) {
        image[i++] = a;
    }

    cl_int err;
    cl_uint num;
    err = clGetPlatformIDs(0, 0, &num);
    if(err != CL_SUCCESS) {
        cerr << "Unable to get platforms\n";
        return 0;
    }
    vector<cl_platform_id> platforms(num);
    err = clGetPlatformIDs(num, &platforms[0], &num);
    if(err != CL_SUCCESS) {
        cerr << "Unable to get platform ID\n";
        return 0;
    }
    cl_context_properties prop[] = { CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platforms[0]), 0 };
    cl_context context = clCreateContextFromType(prop, CL_DEVICE_TYPE_DEFAULT, NULL, NULL, NULL);
    if(context == 0) {
        cerr << "Can't create OpenCL context\n";
        return 0;
    }
    size_t cb;
    clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &cb);
    vector<cl_device_id> devices(cb / sizeof(cl_device_id));
    clGetContextInfo(context, CL_CONTEXT_DEVICES, cb, &devices[0], 0);
    cl_command_queue queue = clCreateCommandQueue(context, devices[0], 0, 0);
    if(queue == 0) {
        cerr << "Can't create command queue\n";
        clReleaseContext(context);
        return 0;
    }

    // create cl_mem by using image data
    cl_mem cl_img = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * input_size, image, NULL);
    cl_mem cl_res = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * 256 * 3, &histogram_results[0], NULL);

    if(cl_img == 0 || cl_res == 0) {
        cerr << "Error for create buffer!" << endl;
        clReleaseMemObject(cl_img);
        clReleaseMemObject(cl_res);
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
        return -1;
    }

    cl_program program = load_program(context, source_code);

    if(program == 0) {
        cerr << "Error for build program!" << endl;
        clReleaseMemObject(cl_img);
        clReleaseMemObject(cl_res);
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
        return -1;
    }

    cl_kernel histogram = clCreateKernel(program, "cl_histogram", 0);
    if(histogram == 0) {
        cerr << "Error for build program!" << endl;
        clReleaseProgram(program);
        clReleaseMemObject(cl_img);
        clReleaseMemObject(cl_res);
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
        return -1;
    }
    clSetKernelArg(histogram, 0, sizeof(cl_mem), &cl_img);
    clSetKernelArg(histogram, 1, sizeof(cl_mem), &cl_res);

    size_t work_size = input_size;
    err = clEnqueueNDRangeKernel(queue, histogram, 1, 0, &work_size, 0, 0, 0, 0);
    if(err == CL_SUCCESS) {
        err = clEnqueueReadBuffer(queue, cl_res, CL_TRUE, 0, sizeof(unsigned int) * 3 * 256, &histogram_results[0], 0, 0, 0);
    }

    for(int c=0; c<256*3; c++) {
        if(c != 0 && c % 256 == 0) {
            outFile << endl;
        }

        outFile << histogram_results[c] << " ";
    }

    clReleaseKernel(histogram);
    clReleaseProgram(program);
    clReleaseMemObject(cl_img);
    clReleaseMemObject(cl_res);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    inFile.close();
    outFile.close();

    return 0;
}

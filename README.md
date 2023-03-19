# Matrix
This program is intended to be an example of a simple compression-type algorithm that utilizes the redundancy that is typical in most matrix-like
data structures. If a value is to be discarded, or ignored, for the sake of saving memory in storage and access, then it can be specified. The actual 
algorithm keeps that value within a struct and builds two different singly-linked lists for the columns and rows which contain non fill-values. The
entire allocation of memory is done on the heap. This can be done by passing a file to the program, or by acting on another, already allocated, matrix
struct. The reassembly of the matrix, with fill-values, can also be done and printed into a specified output file. 

The purpose of this repository is to show an academic exercise. It is not intended, nor was it designed, to be used in practice.

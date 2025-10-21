# Makefile for Producer-Consumer Problem
# Author: Haleigh Harris
# Course: CS 33221 - Programming Assignment 1
# Description: Builds producer and consumer programs using POSIX threads and shared memory

CC = gcc
CFLAGS = -Wall -pthread -lrt

# Targets
all: producer consumer

# Build producer
producer: producer.c
	$(CC) $(CFLAGS) producer.c -o producer

# Build consumer
consumer: consumer.c
	$(CC) $(CFLAGS) consumer.c -o consumer

# Clean up build files
clean:
	rm -f producer consumer *.o

# Rebuild from scratch
rebuild: clean all

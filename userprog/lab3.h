/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lab3.h
 * Author: pintos
 *
 * Created on December 7, 2017, 5:20 AM
 */

#ifndef LAB3_H
#define LAB3_H

#include "userprog/process.h"
#include "threads/lock.h"

struct lock syscallLock;
void sys_exit(int);
void open_handler(struct intr_frame *f);
void create_handler(struct intr_frame *f);
void close_handler(struct intr_frame *f);

void write_handler(struct intr_frame *);
void exit_handler(struct intr_frame *);


#endif /* LAB3_H */


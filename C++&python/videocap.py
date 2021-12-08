#!/bin/python
import cv2
import numpy as np
def writecolor(m,a=[]):
    s=[]
    for i in range(m*9,(m+1)*9):
        H=a[i][0]
        S=a[i][1]
        V=a[i][2]
        if 90<=H<=130 and 40<=S<=255:
            s.append('B')
        elif (0<=H<=135) and 0<=S<=40 and 120<=V<=255:
            s.append('W')
        elif 46<=H<=89:
            s.append('G')
        elif 16<=H<=45:
            s.append('Y')
        elif (150<=H<=180 or 0<=H<=15) and 41<=S<=255 and 0<=V<=169:
            s.append('R')
        elif (150<=H<=180 or 0<=H<=15) and 41<=S<=255 and 170<=V<=255:
            s.append('O')
        else:
            s.append('N')
    fo=open("CUBE_STATE.txt","a")
    for i in range(3):
        fo.write("%s%s%s\n"%(s[3*i],s[3*i+1],s[3*i+2]))
        if i==2:
            fo.write("\n")
    fo.close()
    
    
    
cap=cv2.VideoCapture(0)
n=0
color=[]    
while(cap.isOpened() and n<6):
    ret,frame=cap.read()    
    if ret==True:
        frame=cv2.flip(frame,1)
        cv2.rectangle(frame,(140,60),(230,150),(255,255,255),2)
        cv2.rectangle(frame,(140,170),(230,260),(255,255,255),2)
        cv2.rectangle(frame,(140,280),(230,370),(255,255,255),2)
        cv2.rectangle(frame,(250,60),(340,150),(255,255,255),2)
        cv2.rectangle(frame,(250,170),(340,260),(255,255,255),2)
        cv2.rectangle(frame,(250,280),(340,370),(255,255,255),2)
        cv2.rectangle(frame,(360,60),(450,150),(255,255,255),2)
        cv2.rectangle(frame,(360,170),(450,260),(255,255,255),2)
        cv2.rectangle(frame,(360,280),(450,370),(255,255,255),2)
        cv2.imshow("frame",frame)
        if cv2.waitKey(1)&0xFF== ord('q'):
            kernel = np.ones((5,5),np.uint8)
            frame = cv2.erode(frame,kernel,iterations = 1) 
            frame=cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
            if cv2.waitKey(0)&0xFF== ord('q'):
                color.append(frame[105,405])
                color.append(frame[105,295])
                color.append(frame[105,185])
                color.append(frame[215,405])
                color.append(frame[215,295])
                color.append(frame[215,185])
                color.append(frame[325,405])
                color.append(frame[325,295])
                color.append(frame[325,185])
                n+=1
            else:
                continue
cap.release()
fo=open("CUBE_STATE.txt","w")
fo.close()
for i in range(6):
    writecolor(i,color)
cv2.destroyAllWindows()



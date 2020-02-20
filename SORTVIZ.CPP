
/*
//////////////////////////////////////////////////////////////////////////////////////

	file name:  SORTVIZ.H 
	version: 	1.0

	License (SVIZ-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2019 

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:

	1. Redistributions or derivations of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce the above
	copyright notice. This list of conditions and the following	disclaimer must be
	reproduced in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may
	be used to endorse or promote products derived from this software without specific
	prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS	"AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL,	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
	SUCH DAMAGE. 



	Authors
	~~~~~~ 
	Achyut Timilsena   	(01/BEX/074)
	Nabin Dangi			(10/BEX/074)
	Rohan Shrestha		(18/BEX/074)
	Ronak Hada			(19/BEX/074)

//////////////////////////////////////////////////////////////////////////////////////
*/





//Including some of the important hader files.
#include<iostream.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<graphics.h>
#include <math.h>

//Required global variable.
int rec = 0;


//Union for handling mouse events.
union REGS in, out;

//It initilizes the mouse pointer by giving interrupt 0.
int initmouse()
{
	/*
	** 0 is the interrupt for initializing the mouse in graphics.h header file.
	** If the function returns 1 then mouse is initialized.
	** But is the return value is zero then the mouse is not initialized.
	*/
	in.x.ax = 0;
	int86(0X33, &in, &out);
	return ( out.x.ax );
}

//It shows the mouse pointer by giving interrupt 1.
void showmouseptr()
{
	/*
	** 1 is the interupt for showing mouse pointer.
	*/
   in.x.ax = 1;
   int86(0X33, &in, &out);
}

//Returns the mouse positon
void getmousepos(int *button, int *x, int *y)
{
	/* 
	** 3 is the interrupt for getting mouse button.
	** The register 'bx' stores the number of mouse button clicked, 
	** i.e 1 for left mouse button, 2 for right mouse button.
	** The register 'cx' stores the x coordinates of the mouse position.
	** The register 'dx' stores the y coordinates of the mouse position.
	*/
	in.x.ax = 3;
	int86(0x33, &in, &out);
	*button = out.x.bx;
	*x = out.x.cx;
	*y = out.x.dx;
}
//Draws line by colouring lines during sorting process.
void drawLines(int *lines, int num, int *i1 =NULL,
			   int *i2 = NULL, int *min = NULL, int quick = 0)
{
	setcolor(BLUE);
	rectangle(0, 0, getmaxx(), getmaxy());
	
	/*  
	** The global variable rec is set to 1 only when the following menue is to be
	** printed on the screen.
	*/
	if( rec == 1)
	{
		setcolor(BLUE);
		outtextxy(10, 10, "BLUE  = Current minimum value or the pivot.");
		setcolor(RED);
		outtextxy(10, 20, "RED   = Values being compared.");
		setcolor(GREEN);
		outtextxy(10, 30, "GREEN = Sorted.");
		setcolor(MAGENTA);
	}
	int xref = 10, i, z = 10;
	for(i = 0; i < num; ++i)
	{
		
		/*
		** i1 and i2 are the array position of the two values being compared,
		** so they are coloured red.
		*/
		if(i == *i1 || i == *i2)
		{
			setcolor(RED);
			line(xref + (z * i) , getmaxy() - 10,xref + (z * i),
					getmaxy() - 10 - lines[i]);
		}

		/*
		** The pivot is coloured blue for the insertion and the selection sort..
		** This is used for algorithms other then quick.
		*/
		else if(i == *min && quick == 0)
		{
			setcolor(BLUE);
			line(xref + (z * i) , getmaxy() - 10, xref + (z * i),
				getmaxy() - 10 - lines[i]);

		}

		/*
		** This block is executed for Quick Sort.
		** During partition the pivot is painted blue.
		*/
		else if(i != *min && quick == 1)
		{
			setcolor(BLUE);
			line(xref + (z * *min) , getmaxy() - 10, xref + (z * *min),
				getmaxy() - 10 - lines[*min]);
			setcolor(GREEN);
			line(xref + (z * i) , getmaxy() - 10,xref + (z * i),
					getmaxy() - 10 - lines[i]);

		}

		/*
		** For other other conditions the lines are coloured green are assumed
		** sorted.	
		*/
		else
		{
			setcolor(GREEN);
			line(xref + (z * i) , getmaxy() - 10, xref + (z * i),
					getmaxy() - 10 - lines[i]);
		}
		delay(10);
	}
	delay(100);
}

//Draws line before or after sorting process.
void drawlines(int *lines, int num)
{
	int xref = 10, i, z = 10;
	for(i = 0; i < num; i++)
	{
		setcolor(GREEN);
		line(xref + (z * i) , getmaxy() - 10, xref + (z * i),
					getmaxy() - 10 - lines[i]);
		delay(1);
	}
}

//Selecion Sort Function.
void selectionSort(int *lines, int n)
{
	int i, j, loc, min, temp;

	// One by one move boundary of unsorted subarray
	for (i = 0; i < n; i++)
	{

		// Find the minimum element in unsorted array
		loc = i;
		min = lines[loc];
		for (j = i+1; j < n; j++)
		{
			if (min > lines[j])
			{
				//Now the j is the position of the minimum value.
				min = lines[j];
				loc = j;
			}
			cleardevice();
			setbkcolor(YELLOW);
			setcolor(3);
			outtextxy(10, 60, "->>Selection Sort:");
			drawLines(lines, n, &i, &j, &loc);
			delay(50);
	}
	// Swap the found minimum element with the first element
	temp = lines[i];
	lines[i] = lines[loc];
	lines[loc] = temp;
	}
}

//Bubble Sort Function.
void bubbleSort(int *lines, int num)
{
	int temp, i, j;
	for(i = 0; i < num; i++)
	{
		for(j = i + 1; j < num; j++)
		{
			if(lines[j] < lines[i])
			{
				cleardevice();
				setbkcolor(YELLOW);
				setcolor(3);
			outtextxy(10, 60, "->>Bubble Sort:");
				drawLines(lines, num, &i, &j);
				delay(50);
				//Swappig.
				temp = lines[j];
				lines[j] = lines[i];
				lines[i] = temp;
			cleardevice();
			setbkcolor(YELLOW);
			setcolor(3);
			outtextxy(10, 60, "->>Bubble Sort:");
			drawLines(lines, num, &i, &j);
			delay(50);
			}
			else
			{
			cleardevice();
			setbkcolor(YELLOW);
			setcolor(3);
			outtextxy(10, 60, "->>Bubble Sort:");
			drawLines(lines, num, &i, &j);
			delay(50);
			}
		}
	}
}

//Insertion sort function.
void insertionSort(int* lines, int num)
{
	int i, pivot, temporary, j;
	for (i = 1; i < num; i++)
	{
		pivot = lines[i];
		j = i - 1;
		while (j >= 0 && lines[j] > pivot)
		{
			cleardevice();
			setbkcolor(YELLOW);
			setcolor(3);
			temporary = j + 1;
			outtextxy(10, 60, "->>Insertion Sort:");
			drawLines(lines, num, &i, &j, &temporary);
			lines[j + 1] = lines[j];
			lines[j] = pivot;
			j--;
		}
		lines[j + 1] = pivot;
	}
	drawlines(lines, num);
}

//Function to print the menu.
void printmenu()
{
	setbkcolor(YELLOW);
	outtextxy(10, 10, "Enter your choice:: ");
	outtextxy(20, 30, "(1) Selection Sort.");
	outtextxy(20, 40, "(2) Bubble Sort.");
	outtextxy(20, 50, "(3) Insertion Sort.");
	outtextxy(20, 60, "(4) Quick sort.");
}


//Partition function for Quick sort algorithm.
int partition(int *lines, int start, int end)
{
	int piviot = lines[end];
	int pind = start;
	int i, t;
	for(i = start; i < end; i++)
	{
		if(lines[i] <= piviot)
		{
			cleardevice();
			setbkcolor(YELLOW);
			setcolor(3);
			outtextxy(10, 60, "->>Quick Sort:");
			drawLines(lines, end, &i, &pind, &end, 1);
			delay(50);

			t = lines[i];
			lines[i] = lines[pind];
			lines[pind] = t;
			pind++;
		}
		else
		{
			cleardevice();
			setbkcolor(YELLOW);
			setcolor(3);
			outtextxy(10, 60, "->>Quick Sort:");
			drawLines(lines, end, &i, &pind, &end, 1);
			delay(50);
			}
	}

	t = lines[end];
	lines[end] = lines[pind];
	lines[pind] = t;
	return pind;
}


//Quick sort function.
void quickSort(int *lines, int start, int end)
{
	if(start < end)
	{
		int pind = partition(lines, start, end);
		quickSort(lines, start, pind-1);
		quickSort(lines, pind+1, end);
	}
	drawlines(lines, end);
}



//Main function.
int main()
{
	int number, i, gd = DETECT, gm, choice, button, x, y;
	int select = 0, insert = 0, bubble = 0, quick = 0, unselect = 0;
	int num = 50;
	initgraph(&gd, &gm, "c:\\turboc3\\BGI");
	setbkcolor(YELLOW);
	setcolor(BLUE);
	rectangle(0, 0, getmaxx(), getmaxy());
	int *lines = new int[num];
	/* initialize random seed: */
	srand (time(NULL));
	for(i = 0; i < num; i++)
	{
		number = rand() / 100;
		if(number < 5)
		{
			i--;
		}
		else
		{
			lines[i] = number;
		}
	}
	cleardevice();
	drawlines(lines, num);
	printmenu();
	drawlines(lines, num);

	//This checks if the mouse pointer is initialized by eveluating the return value.
	if ( initmouse() == 0 )
	{
		printf("Mouse support not available.\n");
	}

	//This block is executed if the mouse is initialized. 
	else
	{
		showmouseptr();
		rec = 1;
		getmousepos(&button, &x, &y);
		//This loop will be continued until keyboard key is hit,
		// i.e. kbhit = keyboard hit.
		while(!kbhit())
		{
			/*
			** The select, insert, bubble and quick variables indicated the selection
			** of respectiovely selection, insertion, bubble and quick sort algorithms.
			** If any one of them is '1' then other remaining will be low and the clear
			** device action will not be taken. If the mouse pointer is not pointing to
			** anyone of the options then all of them will be reset to '0' and unselect
			** be set to '1'.
			*/
			getmousepos(&button, &x, &y);

			//This block selects the selection sort option.
			if((x >= 20 && x <= 170) && (y >= 30 && y <= 35))
			{

				if(select != 1)
				{
					cleardevice();
					printmenu();
					drawlines(lines, num);
					rectangle(19, 29, 171, 39);
					select = 1;
					insert = 0;
					bubble = 0;
					quick = 0;
					delay(30);
				}
				else
				{
					printmenu();
					drawlines(lines, num);
					rectangle(19, 29, 171, 39);
					delay(30);
				}

			}

			//This block selects the bubble sort option.
			else if((x >= 20 && x <= 170) && (y >= 40 && y <= 45))
			{

				if(bubble != 1)
				{
					cleardevice();
					printmenu();
					drawlines(lines, num);
					rectangle(19, 39, 171, 49);
					select = 0;
					insert = 0;
					bubble = 1;
					quick = 0;
					delay(30);
				}
				else
				{
					printmenu();
					drawlines(lines, num);
					rectangle(19, 39, 171, 49);	
					delay(30);
				}
			
			}

			//This block selects the inserton sort option.
			else if((x >= 20 && x <= 170) && (y >= 50 && y <= 55))
			{
			
                if(insert != 1)
				{
					cleardevice();
					printmenu();
					drawlines(lines, num);
					rectangle(19, 49, 171, 59);
					select = 0;
					insert = 1;
					bubble = 0;
					quick= 0;
					delay(30);
				}
				
				else
				{
					printmenu();
					drawlines(lines, num);
					rectangle(19, 49, 171, 59);
					delay(30);
				}				
			}

			//This block selects the quick sort option.
			else if((x >= 20 && x <= 170) && (y >= 60 && y <= 65))
			{
				if(quick != 1)
				{
					cleardevice();
					printmenu();
					drawlines(lines, num);
					rectangle(19, 59, 171, 69);
					select = 0;
					insert = 0;
					bubble = 0;
					quick = 1;
					delay(30);
				}
				else
				{
					printmenu();
					drawlines(lines, num);
					rectangle(19, 59, 171, 69);
					delay(30);
				}
			}

			//This block is executed if the mouse doesn't point to any options.
			else
			{
				if(select == 1 || insert == 1 || bubble == 1 || quick == 1)
				{
					cleardevice();
					select = 0;
					insert = 0;
					bubble = 0;
					quick  = 0;
				}
				if(unselect != 0)
				{
					cleardevice();
					printmenu();
					drawlines(lines, num);
					unselect = 1;
					delay(30);

				}
				else
				{
					printmenu();
					drawlines(lines, num);
					delay(30);
				}
			}

			//This block is executed if the left button is clicked.
			if(button == 1)
			{
				button = -1;

				//This proceeds to selection sort algorithm.
				if((x >= 20 && x <= 170) && (y >= 30 && y <= 35))
				{
					rectangle(19, 29, 171, 39);
					delay(500);
					selectionSort(lines, num);
					break;
				}

				//This proceeds to bubble sort algorithm.
				else if((x >= 20 && x <= 170) && (y >= 40 && y <= 45))
				{
					rectangle(19, 39, 171, 49);
					delay(500);
					bubbleSort(lines, num);
					break;
				}

				//This proceeds to insertion sort algorithm.
				else if((x >= 20 && x <= 170) && (y >= 50 && y <= 55))
				{
					rectangle(19, 49, 171, 59);
					delay(500);
					insertionSort(lines, num);
					break;
				}

				//This proceeds to quick sort algorithm.
				else if((x >= 20 && x <= 170) && (y >= 60 && y <= 65))
				{
					rectangle(19, 59, 171, 69);
					delay(500);
					quickSort(lines, 0, num);
					break;
				}

				//This proceeds to noting and remains at the same condition.
				else
				{
					cleardevice();
					drawlines(lines, num);
					printmenu();
				}
				drawlines(lines, num);
			}
			/*
			** If the right button is clicked then no action is taken.
			*/
			else if(button == 2)
			{
				button = -1;
				cleardevice();
				drawlines(lines, num);
				printmenu();
				drawlines(lines, num);
			}
		}
	}
	rec = 1;
	getch();
	closegraph();
	return 0;
}

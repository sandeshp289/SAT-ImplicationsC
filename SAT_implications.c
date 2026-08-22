/////////////////////////////////////////////////////////////////////////////////////
// File       : SAT_implications.c
// Language   : C/C++
// Description: SAT based implications 
// Author     : Sandesh Prabhakar
// Created    : February 13, 2009
// Modified   : August 21, 2026
// Version    : 1.0
//
// Copyright (c) 2026 Sandesh Prabhakar
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
        int value;
	node *link;
};

////////////////////////////////////////////////////////////////////////
// circuit class
////////////////////////////////////////////////////////////////////////

class circuit
{
    node *implications; 
    node **clause;
    node *clause_temp;

    // you may wish to add more private variables here
public:
    circuit(char *, node *);            // constructor

    // you may wish to add more function declarations (or public variables) here
};

////////////////////////////////////////////////////////////////////////
// constructor: reads in the *.cnf file and builds basic data structure
// 		for the gate-level ckt
////////////////////////////////////////////////////////////////////////
circuit::circuit(char *cktName, node *signal_assignment)
{
    FILE *inFile;
    char fName[40];
    int i, j=0, count;
    char c, junk_char;
    int gatenum, junk;
    int f1, f2, f3;
    int variable_count, clause_count; 
    int temp_clause;
    int flag=0, exit_state=0, same_implications=0, zero_flag=0, empty_flag=0, exit_flag=0;

    implications=new node;
    implications=NULL;
    clause_temp=new node;
    clause_temp=NULL;

    strcpy(fName, cktName);
    strcat(fName, ".cnf");
    inFile = fopen(fName, "r");
    if (inFile == NULL)
    {
	fprintf(stderr, "Can't open .cnf file\n");
	exit(-1);
    }

    while((c = getc(inFile)) != '\n')
    {
    fscanf(inFile, "%c", &junk_char); 
    fscanf(inFile, "%d", &variable_count);	// number of variables 
    fscanf(inFile, "%d", &clause_count);	// number of clauses 
    }

    clause=new node*[clause_count+1];
    for(i=0;i<=clause_count;i++)
    clause[i]=NULL;

    for(i=1;i<=clause_count;i++)
    {

    fscanf(inFile, "%d", &temp_clause);

    while(temp_clause!=0)
    {
    zero_flag=0;
    if(flag==0)
    {
      node *current_signal=signal_assignment;

      while(current_signal!=NULL) 
       {
        if(temp_clause==current_signal->value)
	{
        flag=1;
        break;   
	}
       
        else
	{
           if(temp_clause==(-(current_signal->value)))
	   {
           zero_flag=1; 
	   }
           flag=0; 
	}

       current_signal=current_signal->link;  

       }//while current_signal ends

      if(zero_flag==0)
      {
            node *newlink = new node;
	    newlink->value = temp_clause;
	    newlink->link = clause[i];
            clause[i] = newlink;
            j++;
      }

    }//if flag==0 ends
    fscanf(inFile, "%d", &temp_clause);
    
    }//while temp_clause ends
    
   if(flag==1)
      {
      clause[i]=NULL;
      //break;
      }
   same_implications=0;
   if(flag==0)
   {
    if(j==1)
    {
      node *current=implications;
        while(current!=NULL)
        {
           if(current->value==(-(clause[i]->value)))
            {
            printf("\n Application of assignment caused a conflict in formula \n");
            implications=NULL;
            exit_state=1;
            break;
            }
            else if(current->value==clause[i]->value)
            {
            same_implications=1; 
            break;
            }
          current=current->link;
        }

        if(exit_state==1)
        break; 

	if(same_implications==0)
	{
        node *newlink = new node;
	newlink->value = clause[i]->value;
	newlink->link = implications;
        implications = newlink;
        clause[i]=NULL;
	}
    }

    else if(j==0)
    {
       printf("\n Application of assignment caused a conflict in formula \n");
       implications=NULL;
       exit_state=1;
       break;
    }
   }//if flag==0 ends

   flag=0; j=0; 
 }//for i =1, clause count ends

fclose(inFile);
    
   node *current_implications=implications;
   node *current_signal_assign=new node;
         current_signal_assign=NULL;
    while(current_implications!=NULL)
    {
        node *newlink = new node;
	newlink->value = current_implications->value ;
	newlink->link = current_signal_assign ;
        current_signal_assign = newlink;
    
    current_implications=current_implications->link;
    }
    current_implications=NULL;

if(exit_state==0)
{
   while(current_signal_assign!=NULL)
   {
    flag=0; j=0;

    for(i=1;i<=clause_count;i++)
       {
    flag=0;
    empty_flag=0;
    while(clause[i]!=NULL)
    {
    empty_flag=1;
    zero_flag=0;
    if(flag==0)
    {
      node *current_signal=current_signal_assign;

      while(current_signal!=NULL) 
       {
        if(clause[i]->value==current_signal->value)
	{
        flag=1;
        break;   
	}
       
        else
	{
           if(clause[i]->value==(-(current_signal->value)))
	   {
           zero_flag=1; 
	   }
           flag=0; 
	}

       current_signal=current_signal->link;  

       }//while current_signal ends

      if(zero_flag==0)
      {
            node *newlink = new node;
	    newlink->value = clause[i]->value;
	    newlink->link = clause_temp;
            clause_temp = newlink;
            j++;
      }

    }//if flag==0 ends
    
    clause[i]=clause[i]->link;
    }//while clause[i] ends
    
   if((flag==1)&&(empty_flag==1))
      {
      clause[i]=NULL;
      }

   same_implications=0;
   if((flag==0)&&(empty_flag==1))
   {
    if(j==1)
    {
      node *current=implications;
        while(current!=NULL)
        {
           if(current->value==(-(clause_temp->value)))
            {
            implications=NULL;
            exit_flag=1;
            clause_temp=NULL;
            break;
            }
            else if(current->value==clause_temp->value)
            {
            same_implications=1; 
             clause_temp=NULL;
            break;
            }
          current=current->link;
        }

        if(exit_flag==1)
	{
         clause_temp=NULL;
        break; 
	}

	if(same_implications==0)
	{
        node *newlink = new node;
	newlink->value = clause_temp->value;
	newlink->link = implications;
        implications = newlink;
       
        node *newlink_temp = new node;
	newlink_temp->value = clause_temp->value;
	newlink_temp->link = current_implications;
        current_implications = newlink_temp;
        clause[i]=NULL;
	}
    }
    else if(j==0)
    {
       implications=NULL;
       exit_flag=1;
       clause_temp=NULL;
       break;
    }
   else
   {
     clause[i]=NULL;
     while(clause_temp!=NULL)
     {
        node *newlink = new node;
	newlink->value = clause_temp->value;
	newlink->link = clause[i];
        clause[i] = newlink; 

        clause_temp=clause_temp->link;
     }
    
   }

   }//if flag==0 ends
   clause_temp=NULL;
   flag=0; j=0;
  }//for i ends

      if(exit_flag==1)
        break; 

    current_signal_assign=NULL;
     while(current_implications!=NULL)
    {
        node *newlink = new node;
	newlink->value = current_implications->value ;
	newlink->link = current_signal_assign ;
        current_signal_assign = newlink;

    current_implications=current_implications->link;
    }
   current_implications=NULL;
    
   }//while

}//if exit_state==0
   
    printf("\n\tSuccessfully read in the CNF file:\n");
    printf("\t%d total number of variables.\n", variable_count);
    printf("\t%d total number of clauses.\n\n", clause_count);

if(exit_flag==0)
{
    printf("\tThe implications are: \n");
    printf("\t---------------------- \n\n");

   while(implications!=NULL)
    {
     printf("\t%d  ",implications->value); 
     implications=implications->link;
    }
   printf("\n \n");
}
else if(exit_flag==1)
printf("\n Application of assignment caused a conflict in formula \n");

}

////////////////////////////////////////////////////////////////////////
// main starts here
////////////////////////////////////////////////////////////////////////
main(int argc, char *argv[])
{
    char cktName[20];
    circuit *ckt;
    int vecWidth, poNum;
    int i;
    node *signal_assignment; 

    signal_assignment=new node;
    signal_assignment=NULL;

    if (argc <= 2)
    {
        fprintf(stderr, "Usage: %s <ckt> <assignments>...\n", argv[0]);
        exit(-1);
    }

    strcpy(cktName, argv[1]);

    for(i=2;i<argc;i++)
     {
	node *newlink = new node;
	newlink->value = atoi(argv[i]);
	newlink->link = signal_assignment;
        signal_assignment = newlink;
     }
    // read in the circuit and build data structure
    ckt = new circuit(cktName, signal_assignment);
}


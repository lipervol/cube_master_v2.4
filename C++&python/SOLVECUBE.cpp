#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <cstdio>
#include <stdio.h>
#include <windows.h>
using namespace std;

char cube[9][12];       

const int

start_x[6]={3,0,3,3,6,3}, 
start_y[6]={3,3,0,6,3,9},
 
edge_x[24]={2,3,1,3,0,3 ,1,3, 6,5,7,5,8,5, 7,5,4,4,4,4,4, 4,4 ,4},
edge_y[24]={4,4,5,7,4,10,3,1, 4,4,5,7,4,10,3,1,5,6,3,2,9, 8,11,0},
apex_x[24]={2,3,3,0,3,3, 0,3, 3,2,3,3,6,5, 5,6,5,5,8,5,5, 8,5 ,5},
apex_y[24]={5,5,6,5,8,9, 3,11,0,3,2,3,5,6, 5,3,3,2,3,0,11,5,9 ,8};

int func(char ch)
{
    if(ch=='W')
        return 1;
    if(ch=='G')
        return 0;
    if(ch=='O')
        return 2;
    if(ch=='R')
        return 3;
    if(ch=='B')
        return 5;
    if(ch=='Y')
        return 4;
    return 0;
}

char convert(char ch)
{
    if(ch=='W')
        return 'U';
    if(ch=='G')
        return 'F';
    if(ch=='O')
        return 'L';
    if(ch=='R')
        return 'R';
    if(ch=='B')
        return 'B';
    if(ch=='Y')
        return 'D';
  return 0;
}


//----------------------------------------------------------------------

typedef vector<int> vi;

//----------------------------------------------------------------------

int applicableMoves[] = { 0, 262143, 259263, 74943, 74898 };

// TODO: Encode as strings, e.g. for U use "ABCDABCD"

int affectedCubies[][8] = 
{
    {  0,  1,  2,  3,  0,  1,  2,  3 },   // U
    {  4,  7,  6,  5,  4,  5,  6,  7 },   // D
    {  0,  9,  4,  8,  0,  3,  5,  4 },   // F
    {  2, 10,  6, 11,  2,  1,  7,  6 },   // B
    {  3, 11,  7,  9,  3,  2,  6,  5 },   // L
    {  1,  8,  5, 10,  1,  0,  4,  7 },   // R
};

vi applyMove ( int move, vi state ) 
{
  int turns = move % 3 + 1;
  int face = move / 3;
  while( turns-- )
  {
    vi oldState = state;
    for( int i=0; i<8; i++ )
    {
      int isCorner = i > 3;
      int target = affectedCubies[face][i] + isCorner*12;
      int killer = affectedCubies[face][(i&3)==3 ? i-3 : i+1] + isCorner*12;;
      int orientationDelta = (i<4) ? (face>1 && face<4) : (face<2) ? 0 : 2 - (i&1);
      state[target] = oldState[killer];
      state[target+20] = oldState[killer+20] + orientationDelta;
      if( !turns )
        state[target+20] %= 2 + isCorner;
    }
  }
  return state;
}

int inverse ( int move ) 
{
    return move + 2 - 2 * (move % 3);
}

//----------------------------------------------------------------------

int phase;

//----------------------------------------------------------------------

vi id ( vi state ) 
{
  
    //--- Phase 1: Edge orientations.
    if( phase < 2 )
      return vi( state.begin() + 20, state.begin() + 32 );
  
    //-- Phase 2: Corner orientations, E slice edges.
    if( phase < 3 )
    {
      vi result( state.begin() + 31, state.begin() + 40 );
      for( int e=0; e<12; e++ )
          result[0] |= (state[e] / 8) << e;
      return result;
    }
  
    //--- Phase 3: Edge slices M and S, corner tetrads, overall parity.
    if( phase < 4 )
    {
      vi result( 3 );
      for( int e=0; e<12; e++ )
          result[0] |= ((state[e] > 7) ? 2 : (state[e] & 1)) << (2*e);
      for( int c=0; c<8; c++ )
          result[1] |= ((state[c+12]-12) & 5) << (3*c);
      for( int i=12; i<20; i++ )
          for( int j=i+1; j<20; j++ )
        result[2] ^= state[i] > state[j];
      return result;
    }
  
    //--- Phase 4: The rest.
    return state;
}

//----------------------------------------------------------------------
//void sendtocom()
//{ 
//    HANDLE hcom;
//    hcom = CreateFile("COM5",GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING 
//                        ,FILE_ATTRIBUTE_NORMAL,NULL);
//    if (hcom == INVALID_HANDLE_VALUE)
//    {
//       
//		fprintf(stderr, "\n??????????????\n");
//        exit(0);
//    }
//    SetupComm(hcom,1024,1024);
//    DCB dcb;
//    GetCommState(hcom,&dcb);
//    dcb.BaudRate = 9600;
//    dcb.ByteSize = 8;
//    dcb.Parity = 0;
//    dcb.StopBits = 1;
//    SetCommState(hcom,&dcb);
//    FILE *fp;
// 	fp=fopen("SOLUTION.txt","r+");
//	char data[100];
//	fscanf(fp,"%s",&data);
//	printf("%s",data);
//	fclose(fp);
//    DWORD dwWrittenLen = 0;
//	int k=0;
//    if(!WriteFile(hcom,data,2,&dwWrittenLen,NULL))
//    {
//       	fprintf(stderr, "\n??????????????\n");
//    }      
//	printf("\n????????????????????");
//}
//----------------------------------------------------------------------
int main () 
{
  freopen("CUBE_STATE.txt","r",stdin);

  freopen("SOLUTION.txt","w",stdout);
  
  memset(cube,0,sizeof(cube));
  
  string tmp[3];
  
  for(int ii=0;ii<6;ii++)
  {
    for(int jj=0;jj<3;jj++)
      cin>>tmp[jj];
    
    int q=func(tmp[1][1]);
     
    for(int i=0;i<3;i++)
      for(int j=0;j<3;j++)
        cube[start_x[q]+i][start_y[q]+j]=tmp[i][j];
  }
  
  string argv[21];
  
  for(int i=0;i<21;i++)
    argv[i]="";
  int index=1;
    
  for(int i=0;i<24;i++)
  {
    argv[index]+=convert(cube[edge_x[i]][edge_y[i]]);        
    
    if(i%2==1)
      index++;
  }
  for(int i=0;i<24;i++)
  {
    argv[index]+=convert(cube[apex_x[i]][apex_y[i]]);
        
    if(i%3==2 && i!=23)
      index++;
  }

  //--- Define the goal.
  string goal[] = { "UF", "UR", "UB", "UL", "DF", "DR", "DB", "DL", "FR", "FL", "BR", "BL",
      "UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR" };

  //--- Prepare current (start) and goal state.
  vi currentState( 40 ), goalState( 40 );
  for( int i=0; i<20; i++ )
  {
    
      //--- Goal state.
      goalState[i] = i;
    
      //--- Current (start) state.
      string cubie = argv[i+1];
      while( (currentState[i] = find( goal, goal+20, cubie ) - goal) == 20)
      {
        cubie = cubie.substr( 1 ) + cubie[0];
        currentState[i+20]++;
      }
  }
  
  //--- Dance the funky Thistlethwaite...
  while( ++phase < 5 )
  {
    
      //--- Compute ids for current and goal state, skip phase if equal.
      vi currentId = id( currentState ), goalId = id( goalState );
      if( currentId == goalId )
          continue;
    
      //--- Initialize the BFS queue.
      queue<vi> q;
      q.push( currentState );
      q.push( goalState );
    
      //--- Initialize the BFS tables.
      map<vi,vi> predecessor;
      map<vi,int> direction, lastMove;
      direction[ currentId ] = 1;
      direction[ goalId ] = 2;
    
      //--- Dance the funky bidirectional BFS...
      while( 1 )
      {
      
          //--- Get state from queue, compute its ID and get its direction.
          vi oldState = q.front();
          q.pop();
          vi oldId = id( oldState );
          int& oldDir = direction[oldId];
      
          //--- Apply all applicable moves to it and handle the new state.
          for( int move=0; move<18; move++ )
          {
            if( applicableMoves[phase] & (1 << move) )
            {
        
                //--- Apply the move.
                vi newState = applyMove( move, oldState );
                vi newId = id( newState );
                int& newDir = direction[newId];
        
                //--- Have we seen this state (id) from the other direction already?
                //--- I.e. have we found a connection?
                if( newDir  &&  newDir != oldDir )
                {
      
                    //--- Make oldId represent the forwards and newId the backwards search state.
                    if( oldDir > 1 )
                    {
                        swap( newId, oldId );
                        move = inverse( move );
                    }
      
                    //--- Reconstruct the connecting algorithm.
                    vi algorithm( 1, move );
                    while( oldId != currentId )
                    {
                      algorithm.insert( algorithm.begin(), lastMove[ oldId ] );
                      oldId = predecessor[ oldId ];
                    }
                    while( newId != goalId )
                    {
                      algorithm.push_back( inverse( lastMove[ newId ] ));
                      newId = predecessor[ newId ];
                    }
      
                    //--- Print and apply the algorithm.
                    for( int i=0; i<(int)algorithm.size(); i++ )
                    {
                        cout << "UDFBLR"[algorithm[i]/3] << algorithm[i]%3+1;
                        currentState = applyMove( algorithm[i], currentState );
                    }
            
                    //--- Jump to the next phase.
                    goto nextPhasePlease;
                }
    
                //--- If we've never seen this state (id) before, visit it.
                if( ! newDir )
                {
                    q.push( newState );
                    newDir = oldDir;
                    lastMove[ newId ] = move;
                    predecessor[ newId ] = oldId;
                }
            }
        }
      }
      nextPhasePlease:
      ;
    }
    fclose(stdin);
	fclose(stdout);
//sendtocom();
return 0;
}

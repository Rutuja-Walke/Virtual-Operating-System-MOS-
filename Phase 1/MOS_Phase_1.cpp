#include<bits/stdc++.h>
using namespace std;

string IPFILE = "job_phase1.txt";

char Memory[100][4];
vector<char> R(4), IR(4); 
int IC=0, SI=3;
int input_ptr=0, output_ptr=0;
bool C=false;
string buffer="";


void print_mem()
{
    cout<<"\n\nMEMORY\n";
    for(int i=0; i<100; i++)
    {
        for(int k=0; k<4; k++)
        {
            cout<<Memory[i][k]<<" ";
        }
        cout<<endl;
    }
}

void clearContents()
{
    // Memory Initialized by '_'
    for(int i=0;i<100;i++)
    {
    	for(int j=0;j<4;j++)
        {
    		Memory[i][j] = '_';
    	}
	}
    buffer.clear();
    R.clear();
    IC=0;
    C = false;
}

void lineReader(string temp)
{
    fstream fout;
    fout.open("output.txt");
    fout.seekg(0, ios::end);
    fout<<temp;
    fout<<"\n";
    fout.close();

}

void READ(int block)
{   
    cout<<"\nGD"<<endl;
    
    //Obtaining data from input file
    fstream fin;
    string line;
    fin.open(IPFILE, ios::in);
    for(int i=0; i<input_ptr; i++)
    {
        getline(fin, line);
    }
    // WE SKIP LINES
    getline(fin, line);// THIS LINE IS PROGRAM CARD
    input_ptr++;
    fin.close();
    
    buffer = line;
    for(int i=0; i<buffer.size() && i<40; i++)
    {
        int a = i/4 + block;     
        int b = i%4;
        Memory[a][b] = buffer[i];
    }
}

void WRITE(int block)
{
    cout<<"\nPD"<<endl;
    string temp = "";
	int i=0;
    while(i<40)
    {
        int a = i/4 + block;     
        int b = i%4;
		if(Memory[a][b] == '_') 
        {
            temp.push_back(' ');
        }
        else
            temp.push_back(Memory[a][b]);
        i++;
    }
    
    lineReader(temp);

}

int TERMINATE(int si)
{
    if(si==3)
        return 1;
    else 
        return 0;
}

void executeUserProgram()
{
    
    while(1)
    {
        //Bringing instr to IR
        for(int i=0; i<4; i++)
        {
            IR[i] = Memory[IC][i];
        }


        //Halt
        if(IR[0]=='H') 
        {
            TERMINATE(SI);
            break;
        }


        //Address Extraction
        cout<<"IR: "<<IR[0]<<" "<<IR[1]<<" "<<IR[2]<<" "<<IR[3]<<endl;
        int t1 = ((int)IR[2]) - ((int)'0');
        int t2 = ((int)IR[3]) - ((int)'0');
        int block = t1*10 + t2;
        cout<<"Start Block: "<<block<<"\n\n";


        //Checking Instruction
        if(IR[0]=='G' && IR[1]=='D') 
        {   
            SI=1;
            READ(block);
        }

        else if(IR[0]=='P' && IR[1]=='D') 
        {   
            SI=2;
            WRITE(block);
        }

        
        else if(IR[0]=='L' && IR[1]=='R')
        {
            cout<<"\n"<<endl;
            for(int i=0; i<4; i++)
            {
                R[i] = Memory[block][i];
            }
        }

        else if(IR[0]=='C' && IR[1]=='R')
        {
            bool f = 0;

            for(int i=0; i<4; i++)
            {
                if(R[i]!=Memory[block][i])
                {
                    f=1;
                    break;
                }
            }

            if(f) C = false;
            else C = true;
        }


        else if(IR[0]=='S' && IR[1]=='R')
        {
            for(int j=0; j<4; j++)
            {
                Memory[block][j] = R[j];
            }
        }

        else if(IR[0]=='B' && IR[1]=='T')
        {   
            if(C==true)
            {
                IC = block-1;   
            }
        }

        //Incrementing program counter
        IC++;
    }

    cout<<"\n\n #############   Program DONE   ########### \n\n";
}

void startExecution()
{
    int IC=0;
    executeUserProgram();
    
    //Leaving two lines for each jobs
    fstream fout;
    fout.open("output.txt");
    fout.seekg(0, ios::end);
    fout<<"\n\n";
    fout.close();
}

void LOAD()
{
    fstream fin;
    string line ;	
    
    while (1) 
    {   
        //Start file handling        
        fin.open(IPFILE, ios::in);
        for(int j=0; j<input_ptr; j++)
        {
            getline(fin, line);    
        }
        getline(fin, line);
        //cout<<"\n\nHELL: "<<line;
    	input_ptr++;
        fin.close();

        
        //For last job in input file
        if(line.substr(0,4)=="$END")
            break;


        //For next job in input file
        if(line.substr(0, 4) =="$AMJ")
        {   
            //Clear Contents for a new JOB
            clearContents();

            //Job started loading
            string prog;
            fin.open(IPFILE, ios::in);
            for(int j=0; j<input_ptr; j++)
            {
                getline(fin, prog);
            }
            getline(fin, prog);
            input_ptr++;
            cout<<"Program Card: "<<prog<<endl;


            int k=0;
            while(prog.substr(0,4)!="$DTA")
            {
            
                //Loading inst to memory!                 
                for(int j=0; j<prog.size(); j++)
                {   
                    if(prog[j]=='H')
                    {
                        Memory[k/4][0] = 'H';
                        k += 3;
                    }
                    else
                    {
                        int a = k/4;
                        int b = k%4;
                        Memory[a][b] = prog[j];
                    }
                    k++;
                }

                getline(fin, prog);
                input_ptr++;
            }
			fin.close();
            
            startExecution();
            //printing memory after every job
            print_mem();    
            
        }

        //Read Input till end of program
        fin.open(IPFILE, ios::in);
        for(int j=0; j<input_ptr; j++)
        {
            getline(fin, line);
        }
        getline(fin, line);
        input_ptr++;

        while(line.substr(0, 4)!="$END")
        {
            getline(fin, line);
            input_ptr++;
        }
        getline(fin, line);
        
    }
    fin.close();

}




int main()
{
    LOAD();  
  	return 0;
}


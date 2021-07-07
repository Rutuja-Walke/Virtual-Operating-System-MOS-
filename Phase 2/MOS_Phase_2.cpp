#include<bits/stdc++.h>
using namespace std;

string IPFILE = "job.txt";

char Memory[300][4];
vector<char> R(4), IR(4); 
int IC=0, SI=3, job =1, PI =0, TI = 0, cnt = 0, TTL = 0, LineLImit = 0, PTR = 0, Fill;
int input_ptr=0, output_ptr=0;
bool C=false;
string buffer="";
set<int>randnos; 

struct PCB
{
    int TTL, TLL, TTC, TLC;
};
struct PCB jobs[10];


void print_mem()
{
    cout<<"Memory for job "<<job<<"\n";
    for(int i=0; i<300; i++)
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
    for(int i=0;i<300;i++)
    {
    	for(int j=0;j<4;j++)
        {
    		Memory[i][j] = '_';
    	}
	}
    buffer.clear();
    R.clear();
    randnos.clear();
    IC=0;
    C = false;
}

void lineReader(string temp)
{
    jobs[cnt].TLC++;
    fstream fout;
    fout.open("output.txt");
    fout.seekg(0, ios::end);
    fout<<temp;
    fout<<"\n";
    fout.close();

}

void READ(int block)
{   
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

int AddressMap(int VA){
	// returns REAL ADDRESS
	
	int RA;
	int PTE = PTR + VA/10;
	if(Memory[PTE][2]=='*' || Memory[PTE][3]=='*')
    {
        // Page fault
        PI=3;
        // Now handle it by generating a new random number to fill
        
        bool flag = true;

        //int Entry = 12;
        int Entry = rand()%30;
    	if(randnos.find(Entry*10)==randnos.end()){
    		randnos.insert(Entry*10);
    	}
    	else{
    		// number already exists
    		while(randnos.find(Entry*10)!=randnos.end()){
    			Entry = rand()%30;
    		}
    	}
    
        string EntryStr = to_string(Entry);
        
         if(EntryStr.size() == 1){
            Memory[PTE][2] = '0';
            Memory[PTE][3] = EntryStr[0];
         }
         else{
            Memory[PTE][2] = EntryStr[0];
            Memory[PTE][3] = EntryStr[1];
         }
        
        RA = Entry*10 + VA%10;
      
    }
    
    else{
    
    	// Page Exists
    	string t = "";
    	t.push_back(Memory[PTE][2]);
    	t.push_back(Memory[PTE][3]);
   
    	int no = stoi(t);
    	RA = no*10 + VA%10;

    }
    
    return RA;
    
}



void executeUserProgram()
{
    vector<string> ins = {"GD","PD","CR","SR","LR","BT"};

    while(1)
    {
        
        // Now start fetching instructions from Fill Index which is First Entry * 10
        
        //Bringing instr to IR
        jobs[cnt].TTC++;
        string Opc_ins = "", Opr_ins = "";
        for(int i=0; i<4; i++)
        {
            IR[i] = Memory[IC][i];
        }

		//Halt is checked first only
	    if(IR[0]=='H') 
	    {
	        TERMINATE(SI);
	        break;
	    }
	   
		//check operand error  (3 digit or negative or characters in place in numbers)
		if(IR[2] == '-' || isdigit(IR[0]) || isalpha(IR[2]) || isalpha(IR[3])){
			PI = 2;
			break;
		}
		
		Opc_ins+= Memory[IC][0];
	    Opc_ins+= Memory[IC][1];
	    
		// check opcode error
		if (find(ins.begin(), ins.end(), Opc_ins) == ins.end()){ // not in vector (2 lettered handled)
			PI = 1;
			break;
		}
		

	    //Address Extraction
	    //cout<<"IR: "<<IR[0]<<" "<<IR[1]<<" "<<IR[2]<<" "<<IR[3]<<endl;
	    int t1 = ((int)IR[2]) - ((int)'0');
	    int t2 = ((int)IR[3]) - ((int)'0');
	    int block = t1*10 + t2;
	    //cout<<"Start Block: "<<block<<"\n\n";
	   
	    int RealAdd = AddressMap(block);
	   


	    //Checking Instruction
	    if(IR[0]=='G' && IR[1]=='D') 
	    {   
	        SI=1;
	        READ(RealAdd);
	        jobs[cnt].TTC++;// Because in Valid Page faults one more time quantum is required
	    }

	    else if(IR[0]=='P' && IR[1]=='D') 
	    {   
	        SI=2;
	        WRITE(RealAdd);
	    }

	    
	    else if(IR[0]=='L' && IR[1]=='R')
	    {
	        cout<<"\n"<<endl;
	        
	        for(int i=0; i<4; i++)
	        {
	            R[i] = Memory[RealAdd][i];
	        }
	    }

	    else if(IR[0]=='C' && IR[1]=='R')
	    {
	        bool f = 0;

	        for(int i=0; i<4; i++)
	        {
	            if(R[i]!=Memory[RealAdd][i])
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
	            Memory[RealAdd][j] = R[j];
	        }
	        jobs[cnt].TTC++;// Because in Valid Page faults one more time quantum is required
	    }

	    else if(IR[0]=='B' && IR[1]=='T')
	    {   
	        if(C==true)
	        {
	            IC = RealAdd-1;   
	        }
	    }

        //Incrementing program counter
        IC++;
    }

    //cout<<"\n\n #############   Program DONE   ########### \n\n";
}

void startExecution()
{
    IC = Fill;
    executeUserProgram();
    
    //Leaving two lines for each jobs
    fstream fout;
    fout.open("output.txt");
    fout.seekg(0, ios::end);
    fout<<"\n\n";
    fout.close();
}

void SetPageTable(int PTR){
	for(int i=0; i<10; i++){
        Memory[PTR+i][0] = '*';
        Memory[PTR+i][1] = '*';
        Memory[PTR+i][2] = '*';
        Memory[PTR+i][3] = '*';
    }
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
    	input_ptr++;
        fin.close();

        
        //For last job in input file
        if(line.substr(0,4)=="$END")
            break;


        //For next job in input file
        if(line.substr(0, 4) =="$AMJ")
        {   
            //Clear Contents for a new JOB
            PI = 0;
            clearContents();
            
            // FInd TTL
            TTL = stoi(line.substr(8,4));
            
            // FInd Line Limit
            LineLImit = stoi(line.substr(12,4));
            
            // Initialize PCB
            struct PCB CJob;
            CJob.TTL = TTL;
            CJob.TLL = LineLImit;
            CJob.TTC = 0;
            CJob.TLC = 0;
            jobs[cnt]= CJob;
            
            // Generate PTR
    
            PTR = rand()%30;
            // Now to prevent random numbers to lie in page table add page table row numbers to a set
            
            PTR = PTR *10;
            for(int i=PTR;i<(PTR+11);i++){
            	randnos.insert(i);
            }// Page Table Avoided
            
            
            // Now initialize page table at PTR*10
            SetPageTable(PTR);
            
            // Generate first entry
 
            int First_Entry = rand()%30; // Allocate Fnction
            
            
            //FIll FIrst entry in row 1 pointed by PTR
            string FE = to_string(First_Entry);
            if(FE.size() == 1){
            	Memory[PTR][2] = '0';
            	Memory[PTR][3] = FE[0];
            }
            else{
            	Memory[PTR][2] = FE[0];
            	Memory[PTR][3] = FE[1];
            }
            
            // Now go to First entry block and start filling program card
            Fill = First_Entry * 10;
            randnos.insert(Fill);
       
             
            
            //Job started loading
            string prog;
            fin.open(IPFILE, ios::in);
            for(int j=0; j<input_ptr; j++)
            {
                getline(fin, prog);
            }
            getline(fin, prog);
            input_ptr++;
            //cout<<"Program Card: "<<prog<<endl;

			// We are now loading Program card
            int k=Fill;
            int b = 0,rw = 0;
            while(prog.substr(0,4)!="$DTA")
            {
            
                //Loading inst to memory!                 
                for(int j=0; j<prog.size(); j++)
                {   
                    randnos.insert(k); // this is to avoid any data to get inserted into program card segment
                    if(prog[j]=='H')
                    {
                        Memory[k][0] = 'H';
                        k++;
                        continue;
                    }
                    
                    else
                    {
		                Memory[k][b] = prog[j];
		                rw++;
                    }
                    if(rw%4 == 0){
                    	k++;
                    	b = -1;
                    	rw = 0;
                    }
                    b++;
                }

                getline(fin, prog);
                
                input_ptr++;
            }
            // Program card loaded
            
            // here line ptr is at the starting part of Data seg
			getline(fin, prog);
			
			// If no data, it will directly point to $END
			if(prog.substr(0,4)=="$END")
            {
				cout<<"Out of Data Error! \n";
			}
			
			fin.close();
            
            startExecution();
         
            // JOB IS DONE ##########################
            
           
            
            //TTL errors
            //cout<<jobs[cnt].TTL<<jobs[cnt].TTC;
            if(jobs[cnt].TTL<jobs[cnt].TTC)
            {
            	TI = 2;
            	cout<<"Time Limit Exceeded! It took "<<jobs[cnt].TTC<<" units of time. Expected TTL was "<<jobs[cnt].TTL<<"\n";
            	//break;
            }
            
            //Line Limit errors
            //cout<<"Line limit"<<jobs[cnt].TLC<<"expected lines "<<jobs[cnt].TLL;
            else if(jobs[cnt].TLL<jobs[cnt].TLC)
            {
            	cout<<"Line Limit Exceeded! It printed "<<jobs[cnt].TLC<<" lines. Expected Line Limit was "<<jobs[cnt].TLL<<"\n";
            	//break;
            }
            
            //PI Errors
            else if(PI == 1)
            {
            	cout<<"Opcode Error: \n";
            	cout<<"JOb No "<<job<<" is not executed \n";    
            }
            else if(PI == 2)
            {
            	cout<<"Operand Error: \n";
            	cout<<"JOb No "<<job<<" is not executed \n";    
            }
            else
            {
            	print_mem();
            }
			job++; // job variable incremented
			cnt++; // pointer for array of PCBs
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


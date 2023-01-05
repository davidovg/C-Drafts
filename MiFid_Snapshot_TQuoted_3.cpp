#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include <iomanip>
#include <sstream>
using namespace std;

string GetStdoutFromCommand(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 250;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
    }
    return data;
    }
    
    
int sendmail(const char *to, const char* cc, const char* cc1, const char *from, const char *subject, const char  *message)
{
    int retval = -1;
    FILE *mailpipe = popen("/usr/lib/sendmail -t", "w");
    if (mailpipe != NULL) {
        fprintf(mailpipe, "To: %s\n", to);
        fprintf(mailpipe, "Cc: %s\n", cc);
        fprintf(mailpipe, "Cc: %s\n", cc1);
        fprintf(mailpipe, "From: %s\n", from);
        fprintf(mailpipe, "Subject: %s\n\n", subject);
        fwrite(message, 1, strlen(message), mailpipe);
        fwrite(".\n", 1, 2, mailpipe);
        pclose(mailpipe);
        retval = 0;
     }
     else {
         perror("Failed to invoke sendmail");
     }
     return retval;
}
    
int main () 
{
  string line[10000], s[10000][10];
  	string cmd, cmd1, icmd, cmd2;
  int i=0, k, j, countRows; 
  //ifstream myfile ("\\p1at.s-group.cc\\0196\\users\\A96KW6B\\Daten\\bypass_softlimits.csv");
  //ifstream myfile ("\\p1at.s-group.cc\\0196\\users\\A96KW6B\\Downloads\\Scripts\\last\\MiFid_Snapshot.csv");
    //ifstream myfile ("C:\\Users\\A96KW6B\\Downloads\\Scripts\\last\\MiFid_Snapshot.csv");
  ifstream myfile ("C:/Users/Davidov/Downloads/MiFid_Snapshot_2.csv");
 //ifstream myfile ("MiFid_Snapshot.csv");
  if (myfile.is_open())
  {
	while (! myfile.eof() )
	{
	  getline (myfile,line[i]);
	  i=i+1;
//      cout<<line[i]<<endl;
  	}
	myfile.close();
  }
  else cout << "Unable to open file";
//  system("PAUSE");
  for (int k=0; k<i; k++) {
    j=0;
    line[k]=line[k]+";";
    do {
  	  std::size_t pos = line[k].find(";");
  	  j=j+1;
  	  s[k][j] = line[k].substr(0,pos);
  	  line[k]=line[k].erase(0,pos+1);
     }
    while (line[k].length()!=0);    
//    cout<<s[k][1]<<" "<<s[k][2]<<" "<<s[k][3]<<endl;
    countRows++;
};
//cout<<countRows;
for (int i = 2; i <= countRows ; i++) {
if (s[i][4].substr(0,7)=="<error>") {
s[i][4]="0";
}}
//sort the instruments in ascending order

   for (int i =1; i <= countRows; i++) // start at 1 and compare with previous
   {
   	int k=i;
   	float min=atof(s[i][4].c_str());
   	for (int j=i+1; j<=countRows-1; j++) {
      if (atof(s[j][4].c_str()) < min)  // move bigger values along, use "<" for descending
      { 
       min = atof(s[j][4].c_str());
       k=j;
   }}
      for (int z =1; z <=4 ; z++) { 
      string swap = s[i][z]; s[i][z]=s[k][z]; s[k][z]= swap;   
      }
   }
 const int isin_width = 12 ;
    const int name_width = 40 ;
    const int venue_width = 5 ;
    const int num_width = 5 ;   
const std::string sep = " |" ;
//build the strings after instruments are sorted
    for (int k=0; k<=countRows; k++) {
		if (s[k][1] != "") {
		//cmd="tblog name "+s[k][j];
		//cout<<cmd;
		//string ccmd = GetStdoutFromCommand(cmd);
        //cout << " captured result of command:\n " << ccmd << endl;
        std::ostringstream buffer1,buffer2, buffer3, buffer4;   
		buffer1 << std::setw(isin_width) << left << s[k][1];    
        string ISIN_str = buffer1.str();
        buffer2 << std::setw(name_width) << left << s[k][2]; 
        string Short_Name_str = buffer2.str();
        //ccmd=ccmd.substr(0, ccmd.find("(", 0)); 
        //if (s[k][j+4] !== "") {
         //       icmd="tblog name "+s[k][j+4];
         //       cout << icmd;  
         //	string iicmd = GetStdoutFromCommand(icmd);
        buffer4 << std::setw(num_width) << left << s[k][4]; 
        string T_Quoted = buffer4.str();
        buffer3 << std::setw(venue_width) << left << s[k][3];
        string Venue = buffer3.str();
        if (s[k][4].substr(0,7)!="<error>") {
        //cout << " captured result of command:\n " << iicmd << endl;
        //iicmd=iicmd.substr(0, iicmd.find("(", 0)); 
         cmd1=cmd1+ ISIN_str + sep + Short_Name_str + sep + Venue + sep +  "Perc. " + T_Quoted  +  "\n";
        }
	};
    };
cmd2="Please find below percentage time qouted of our instruments:\n\n" + cmd1 + "\n *** This is an automatically generated email, please do not reply ***";
//cmd2=cmd2.substr(0, cmd2.find(" ***", 0)); 
   char * writable = new char[cmd2.size()+1];
   std::copy(cmd2.begin(), cmd2.end(), writable);
  // sendmail("Florian.Hanl@erstegroup.com", "Ronald.Nemec@erstegroup.com", "boyan.davidov@erstegroup.com", "ML0664EB@erstegroup.com",  "Report Market Maker Percentage Time Quoted", writable);
   cout << cmd2;
 return 0;
}

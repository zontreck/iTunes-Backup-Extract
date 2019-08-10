/*
	ZNI Extract v1.0
	Copyright (c) 2019 Tara Piccari
	Program description: Extract the SMS history and Call log from your itunes backup SQL databases (backup must not be encrypted)
	Licensed under the GPLv2, see license.txt for more details!

	Per GPLv2, you may make any modifications you'd like to the software so long as it stays opensource! You may not change the license

 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sqlite3.h>
#include <sys/stat.h>

using namespace std;

bool checkFile(string fName){
	struct stat buffer;
	return (stat(fName.c_str(), &buffer)==0);
}

void nfolder(string sName){
	#if defined(_WIN32)
	_mkdir(sName.c_str());
	#else
	mkdir(sName.c_str(),0777);
	#endif

}

void ZEv1(){
	
	cout << " _______   _ _____                "<<endl<<"|___  | \\ | |_   _|               "<<endl<<"   / /|  \\| | | |                 "<<endl << "  / / | . ` | | |                 "<<endl << "./ /__| |\\  |_| |_                "<<endl << "\\_____\\_| \\_/\\___/            _   "<<endl << "|  ___|   | |                | |  "<<endl<<"| |____  _| |_ _ __ __ _  ___| |_ "<<endl<<"|  __\\ \\/ | __| '__/ _` |/ __| __|"<<endl<<"| |___>  <| |_| | | (_| | (__| |_ "<<endl<<"\\____/_/\\__\\__|_|  \\__,_|\\___|\\__|" <<endl<<"        /  |                      "<<endl<<"__   __ `| |                      "<<endl<<"\\ \\ / /  | |                      "<<endl<<" \\ V /  _| |_                     "<<endl<<"  \\_/   \\___/        "<<endl;
	// ^ ^ ^ ZNI Extract v1
}
sqlite3 *g_dbSMS;
sqlite3 *g_dbCalls;

int main(int argc, char* argv[]){
	// BEGIN FILE PROMPTS
	ZEv1();
	
	cout<<endl << endl;
	bool iHasSMS=false;
	string sSMSName;
	while(!iHasSMS){

		cout << "Please enter the filename/location of the 3d0d7e5fb2ce288813306e4d4636395e047a3d28 file [help/%filename]: ";
		string sEntry;
		getline(cin, sEntry);
		if(sEntry.compare("help")==0){
			cout << "The 3d0d7e5fb2ce288813306e4d4636395e047a3d28 file is usually located within your iTunes backups. On windows this is under %appdata%/Apple Computer/MobileSync/Backup/DEVICE_ID/3d/"<<endl;
			cout << "* On mac this file might be located under ~/Library/Application Support/MobileSync/Backup"<<endl;
			cout<<endl;
		}else{
			if(checkFile(sEntry)){

				iHasSMS=true;
				sSMSName = sEntry;
			}else{
				cout << "Please enter a valid filename to proceed **"<<endl<<endl;
			}
		}


	}
	cout<<endl;
	string sCL;
	bool iHasCallLog=false;
	while(!iHasCallLog){
		cout << "Please enter the filename/location of the 5a4935c78a5255723f707230a451d79c540d2741 file [help/%filename]: ";
		string sEntry;
		getline(cin, sEntry);
		if(sEntry.compare("help")==0){
			cout << "The 5a4935c78a5255723f707230a451d79c540d2741 file is usually located within your iTunes backups. On windows this is under %appdata%/Apple Computer/MobileSync/Backup/DEVICE_ID/5a/"<<endl;
			cout << "* On mac this file might be located under ~/Library/Application Support/MobileSync/Backup"<<endl;
			cout<<endl;
		}else{
			if(checkFile(sEntry)){
				iHasCallLog=true;
				sCL = sEntry;
			}else{
				cout << "Please enter a valid filename to proceed **"<<endl<<endl;
			}
		}
	}
	cout<<endl;

	// END FILE PROMPTS

	// PROMPT FOR OUTPUT LOCATION (SMS)
	string sOutSMS;
	iHasSMS=false;
	while(!iHasSMS){
		cout << "What folder should we save the output of text messages to? ";
		string sEntry;
		getline(cin,sEntry);
		if(checkFile(sEntry)){
			cout << "This folder must not exist!"<<endl<<endl;
		}else{
			cout << "Setting folder location as '"<<sEntry.c_str()<<"'"<<endl;
			cout << "*folder will be created\r";
			nfolder(sEntry);
			cout << "*sub_dir [html,txt]\r";
			string htmlDir = sEntry;
			htmlDir.append("/html");
			nfolder(htmlDir);
			htmlDir=sEntry;
			htmlDir.append("/txt");
			nfolder(htmlDir);
			cout << "Folder structure initialized!"<<endl;
			iHasSMS=true;
			sOutSMS=sEntry;
		}
	}

	cout<<endl;
	// END PROMPT

	// PROMPT FOR OUTPUT LOCATION (CALLS)
	string sOutCalls;
	iHasCallLog=false;
	while(!iHasCallLog){
		cout << "What folder should we save the output for calls to? ";
		string sEntry;
		getline(cin,sEntry);
		if(checkFile(sEntry)){
			cout << "This folder must not exist! " <<endl<<endl;
		}else{
			cout <<"Setting folder location as '"<<sEntry<<"'"<<endl;
			cout << "*folder will be created\r";
			nfolder(sEntry);
			cout << "*sub_dir [txt]\r"; // HTML may come later
			string dirx = sEntry;
			dirx.append("/txt");
			nfolder(dirx);


			cout << "Folder structure initialized!"<<endl;
			iHasCallLog=true;
			sOutCalls=sEntry;

		}
	}
	cout<<endl;
	// END PROMPT

	// Initialize sqlite3 SMS
	char *xErr =0;
	int iCode;
	iCode = sqlite3_open(sSMSName.c_str(), &g_dbSMS);
	if(iCode){
		cout << "Can't open database! "<< sqlite3_errmsg(g_dbSMS)<<endl;
		return 1;
	}else{
		cout << "Opened SMS database"<<endl;
	}

	// Initialize sqlite3 calls
	xErr=0;
	iCode=sqlite3_open(sCL.c_str(), &g_dbCalls);
	if(iCode){
		cout << "Can't open database! "<<sqlite3_errmsg(g_dbCalls)<<endl;
		return 2;
	}else{
		cout << "Opened call database"<<endl;
	}

	// Set timeouts
	sqlite3_busy_timeout(g_dbSMS, 60000);
	sqlite3_busy_timeout(g_dbCalls, 60000);

	// Perform SMS queries
	//!-!// This segment sorta chainloads..
	string query = "select * from chat;";
	sqlite3_stmt *sps_chat;
	int select_chat = sqlite3_prepare_v2(g_dbSMS, query.c_str(), -1, &sps_chat, NULL);
	int iPos=0;
	
	while(sqlite3_step(sps_chat)==SQLITE_ROW){
		string Handle;
		int i;
		string Service;
		string chat_identifier;
		
		for(i=0;i<sqlite3_column_count(sps_chat);i++){

			string col = sqlite3_column_name(sps_chat, i);
			if(col.compare("ROWID")==0){
				Handle = string(reinterpret_cast<const char*>(sqlite3_column_text(sps_chat, i)));
			}
			if(col.compare("service_name")==0){
				Service = string(reinterpret_cast<const char*>(sqlite3_column_text(sps_chat,i)));
			}
			if(col.compare("chat_identifier")==0){
				chat_identifier=string(reinterpret_cast<const char*>(sqlite3_column_text(sps_chat, i)));
			}

		}

		//cout << Handle << " - "<<Service << " - "<< chat_identifier<<endl;
		// Build a SQL Query 
		cout << "Building Message history for "<<chat_identifier<<" ...\r"; // Use \r to ensure that we dont spam the console
		string get_msg = "select * from message where handle_id='";
		get_msg.append(Handle);
		get_msg.append("';");
		sqlite3_stmt *sps_msg;
		sqlite3_prepare_v2(g_dbSMS, get_msg.c_str(), -1, &sps_msg, NULL);
		ofstream output;
		string txtDir = sOutSMS;
		txtDir.append("/txt/");
		txtDir.append(chat_identifier);
		txtDir.append(".txt");
		output.open(txtDir.c_str());

		while(sqlite3_step(sps_msg)==SQLITE_ROW){
			string text;
			int is_from_me;
			int ii;
			for(ii=0;ii<sqlite3_column_count(sps_msg);ii++){
				string col = sqlite3_column_name(sps_msg, ii);
				if(col.compare("text")==0){
					text = string(reinterpret_cast<const char*>(sqlite3_column_text(sps_msg,ii)));
				}
				if(col.compare("is_from_me")==0){
					is_from_me=sqlite3_column_int(sps_msg,ii);
				}
			}

			// Write to file
			if(is_from_me ==1){
				output<<"[ME]: "<<text<<"\n";
			}else{
				output << "["<<chat_identifier<<"]: "<< text<<"\n";
			}
			//TODO: Render as HTML
		}
		output.close();
		sqlite3_finalize(sps_msg);
		iPos++;

	}
	sqlite3_finalize(sps_chat);
	// END SMS Queries

	// Close SQLite3 Databases!
	sqlite3_close(g_dbSMS);
	sqlite3_close(g_dbCalls);
	cout << "Databases closed. Exiting                     "<<endl;
	return 0;
}
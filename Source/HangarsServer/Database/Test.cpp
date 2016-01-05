#include <HangarsServerStd.h>
#include <GameLogic/HangarsGameLogic.h>
#include <iostream>
#include "Test.h"

extern void TestDatabase()
{
	String input = 
		"CREATE TABLE hangarsdb.Accounts"
		"("
		    "AccountIdx int NOT NULL AUTO_INCREMENT PRIMARY KEY, "
		    "AccountUsername varchar(45), "
		    "AccountFirstname varchar(45), "
		    "AccountLastname varchar(45), "
		    "AccountEmail varchar(45), "
		    "AccountPassword varchar(45), "
		    "AccountUniqueID varchar(128)"
		")";
	String testinput = "CREATE TABLE `test1` ("
		"`id` INT NULL"
		")"
		"COLLATE = 'latin1_swedish_ci'"
		"ENGINE = InnoDB"
		";";




	//String testinput_1 "hangarsdb.create table tbl1(col1 varchar(10), col2 smallint)";
	URHO3D_LOGINFO(input);
	URHO3D_LOGINFO(testinput);
	DbResult result = g_pApp->GetDatabaseConnection()->Execute(testinput, true);
}
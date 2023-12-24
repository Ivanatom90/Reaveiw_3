DBConnector CreateDBConnector(bool db_allow_exceptions, DBLogLevel db_log_level)
{
   return DBConnector{db_allow_exceptions, db_log_level}
}

optional<DBHandler> CreateDBHandler(string_view db_name, int db_connection_timeout){
	DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_connection_timeout);
    } else {
        db = connector.Connect(db_name, db_connection_timeout);
    }
    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }
return 	db;
}


DBQuery CreateDBQuery(int min_age, int max_age, string_view name_filter, const DBHandler& db){
	    ostringstream query_str;
    	query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
	DBQuery query(query_str.str());
	return query;
}


vector<Person> LoadPersons(string_view db_name, int db_connection_timeout, bool db_allow_exceptions,
                           DBLogLevel db_log_level, int min_age, int max_age, string_view name_filter) {
    
	DBConnector connector = CreateDBConnector(db_allow_exceptions, db_log_level);
	DBHandler db = CreateDBHandler(string_view db_name, int db_connection_timeout);
	if(db == nullsptr){ return{};}	
    DBQuery query = CreateDBQuery(min_age,  max_age, string_view name_filter, DBHandler& db);
    vector<Person> persons;
	
    for (auto& [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}

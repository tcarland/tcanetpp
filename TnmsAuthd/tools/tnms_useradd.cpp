#define _TNMS_USERADD_CPP_

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>

#include "SqlSession.hpp"
using namespace tcasqlpp;

#include "StringUtils.h"
using namespace tcanetpp;

#include "XmlDocument.h"
using namespace tcaxmlpp;


static const
char TNMS_USERADD_VERSION [] = ".11";


const char * process = "tnms_useradd";

const char * _Dbhost  = "localhost";
const char * _Dbuser  = "tnmsauth";
const char * _Dbname  = "tnmsauthd";
const char * _Dbpass  = "tnmsauth11b";


uint32_t addGroup ( SqlSession * sql, const std::string & group );

void
usage()
{
    printf("Usage: %s [-ADhLV] [-u username] [-g group] [-p password] [-m auth_method]\n", process);
    printf("          -A           :  User can be an agent\n");
    printf("          -c <config>  :  XML configuration for user or agent\n");
    printf("          -h           :  Display help info and exit\n");
    printf("          -u <user>    :  Username to add \n");
    printf("          -g <group>   :  Name of user's group \n");
    printf("          -p <passwd>  :  User password (if applicable)\n");
    printf("          -m <method>  :  Authentication method type\n");
    printf("                          Valid methods are: 'securid', 'dbstatic', or 'kerberos'\n");
    printf("          -D           :  Delete an existing user\n");
    printf("          -L           :  List all users\n");
    printf("          -V           :  Display version info and exit\n");
    exit(0);
}

void 
version()
{
    printf("%s Version: %s\n", process, TNMS_USERADD_VERSION);
    exit(0);
}


uint32_t
getAUID ( SqlSession * sql, const std::string & method )
{
    Query     query = sql->newQuery();
    uint32_t  id    = 0;
    Result    res;
    Row       row;

    Result::iterator rIter;

    query << "SELECT authtype_id FROM " << _Dbname << ".auth_types WHERE method_name=\""
          << method << "\"";

    if ( ! sql->submitQuery(query, res) ) {
        std::cout << "Error in authmethod query: " << sql->sqlErrorStr()
            << std::endl;
        return 0;
    }

    if ( res.size() == 1 ) {
        rIter = res.begin();
        row   = (Row) *rIter;
        id    = StringUtils::fromString<uint32_t>(row[0]);
    }

    return id;
}

uint32_t
getGID ( SqlSession * sql, const std::string & group )
{
    uint32_t     id  = 0;

    if ( group.empty() )
        return id;

    Query    query = sql->newQuery();
    Result   res;
    Row      row;

    Result::iterator  rIter;
    
    query << "SELECT gid FROM " << _Dbname << ".groups WHERE name=\"" << group << "\"";

    if ( ! sql->submitQuery(query, res) ) {
        std::cout << "Error in groupname query: " << sql->sqlErrorStr()
                  << std::endl;
        return 0;
    }

   if ( res.size() == 0  ) {
        char  r;
        std::cout << "Group does not exist, create new group '" 
            << group << "'? (y/n) ";
        std::cin >> r;
        if ( r == 'y' )
            id = addGroup(sql, group);
    } else {
        rIter = res.begin();
        row   = (Row) *rIter;
        id    = StringUtils::fromString<uint32_t>(row[0]);
    }

    return id;
}

uint32_t
getUID ( SqlSession * sql, const std::string & user )
{
    uint32_t     id    = 0;
    Query        query = sql->newQuery();
    Result       res;
    Row          row;

    Result::iterator  rIter;

    query << "SELECT uid FROM " << _Dbname << ".users WHERE username=\""
          << user << "\"";

    if ( ! sql->submitQuery(query, res) ) {
        std::cout << "Error querying db: " << sql->sqlErrorStr() << std::endl;
        throw std::runtime_error(sql->sqlErrorStr());
    } else if ( res.size() == 1 ) {
        rIter = res.begin();
        row   = (Row) *rIter;
        id    = StringUtils::fromString<uint32_t>(row[0]);
    }

    return id;
}

uint32_t
addGroup ( SqlSession * sql, const std::string & group ) 
{
    Query  query = sql->newQuery();

    query << "INSERT INTO " << _Dbname << ".groups ( name ) VALUES ( \""
          << group << "\" )";

    if ( ! sql->submitQuery(query) ) {
        std::cout << "Error adding group: " << sql->sqlErrorStr() << std::endl;
        return 0;
    }

    return getGID(sql, group);
}

uint32_t
addUser ( SqlSession * sql, const std::string & user, const std::string & pw, 
          uint32_t gid, uint32_t auid )
{
    Query  query = sql->newQuery();
    uint64_t id  = 0;
    uint32_t uid = 0;

    query << "INSERT INTO " << _Dbname << ".users (username, gid, authtype_id";

    if ( ! pw.empty() )
        query << ", password";

    query << ") " << "VALUES (\"" << user << "\", " << gid << ", "
          << auid;

    if ( ! pw.empty() )
        query << ", \"" << sql->escapeString(pw) << "\"";

    query << ")";

    if ( ! sql->submitQuery(query) ) {
        std::cout << "Error adding user: " << sql->sqlErrorStr() << std::endl;
        return uid;
    }

    id  = sql->insert_id();
    uid = (*(uint32_t*) &id);

    std::cout << " Added user '" << user << "' uid=" << id << std::endl;

    return uid;
}


bool
updateUser ( SqlSession * sql, uint32_t uid, uint32_t gid, uint32_t auid,  
             const std::string & pw, const std::string & method )
{
    Query  query = sql->newQuery();

    if ( method.compare("dbstatic") == 0 && pw.empty() ) {
        std::cout << "password required" << std::endl;
        return false;
    }

    query << "UPDATE " << _Dbname << ".users SET gid=" << gid
          << ", authtype_id=" << auid << ", password=\"" << sql->escapeString(pw)
          << "\" WHERE uid=" << uid;

    if ( ! sql->submitQuery(query) ) {
        std::cout << "Error updating user: " << sql->sqlErrorStr() << std::endl;
        return false;
    }
    std::cout << " Updated user " << uid << std::endl;

    return true;
}


bool
updateUserConfig ( SqlSession * sql, uint32_t uid, const std::string & xmlconfig )
{
	Query  query = sql->newQuery();

	query << "UPDATE " << _Dbname << ".user_configs SET config=\""
	      << sql->escapeString(xmlconfig) << "\" WHERE uid=" << uid;

	if ( ! sql->submitQuery(query) ) {
		std::cout << "Error updating user config: " << sql->sqlErrorStr() << std::endl;
		return false;
	}

	std::cout << " Updated the user configurations for uid " << uid << std::endl;

	return true;
}


bool
deleteUser ( SqlSession * sql, const std::string & user, uint32_t uid )
{
    Query query = sql->newQuery();

    query << "DELETE FROM " << _Dbname << ".users WHERE uid=" << uid;

    if ( ! sql->submitQuery(query) )
    {
        std::cout << "Error deleting user: " << sql->sqlErrorStr() << std::endl;
        return false;
    }
    std::cout << "Deleted user: " << user << std::endl;

    return true;
}

bool
setUserAgent ( SqlSession * sql, uint32_t uid, bool agent )
{
    Query  query = sql->newQuery();

    query << "UPDATE " << _Dbname << ".users SET is_agent=" << agent
          << " WHERE uid=" << uid;

    if ( ! sql->submitQuery(query) ) {
        std::cout << "Error updating user: " << sql->sqlErrorStr() << std::endl;
        return false;
    }
    std::cout << "Updated user agent status for uid " << uid << std::endl;

    return true;
}


bool listUsers ( SqlSession * sql, uint32_t gid )
{
    Query        query = sql->newQuery();
    Result       res;
    Row          row;

    Result::iterator  rIter;

    query << "SELECT u.uid, u.authtype_id, u.gid, u.username, u.is_agent, g.name, "
          << "m.method_name, m.authbin_name FROM " << _Dbname << ".users u JOIN " 
          << _Dbname << ".groups g on g.gid = u.gid "
          << "JOIN " << _Dbname << ".auth_types m on m.authtype_id = u.authtype_id";

    if ( gid > 0 )
        query << " WHERE u.gid=" << gid;

    query << " ORDER BY u.username";

    if ( ! sql->submitQuery(query, res) ) {
        std::cout << "Error listing users: " << sql->sqlErrorStr() << std::endl;
        return false;
    }

    if ( res.size() == 0 ) {
        std::cout << "no results" << std::endl;
        return false;
    }

    std::cout << std::endl;
    std::cout << std::setiosflags(std::ios::left)
         << std::setw(32) << "  user name  "
         << std::setw(8)  << " uid  "
         << std::setw(8)  << " gid  "
         << std::setw(15) << " group name  "
         << std::setw(15) << " authmethod "
         << std::setw(5)  << " agent " << std::endl;

    std::cout << std::setiosflags(std::ios::left)
         << std::setw(32) << "----------------------"
         << std::setw(8)  << "-----"
         << std::setw(8)  << "-----"
         << std::setw(15) << "-------------"
         << std::setw(15) << "-----------"
         << std::setw(5)  << "-----"
         << std::resetiosflags(std::ios::left) << std::endl;

   for ( rIter = res.begin(); rIter != res.end(); ++rIter ) {
        row = (Row) *rIter;

        std::cout << std::setiosflags(std::ios::left)
            << std::setw(32) << ((std::string) row[3]) << " "
            << std::setw(8)  << StringUtils::fromString<int>(row[0])
            << std::setw(8)  << StringUtils::fromString<int>(row[2])
            << std::setw(15) << ((std::string) row[5])
            << std::setw(15) << ((std::string) row[6])
            << std::setw(5)  << ((std::string) row[4])
            << std::endl;
    }
    std::cout << std::resetiosflags(std::ios::left) << std::endl;
    std::cout << std::endl;

    return true;
}



int main ( int argc, char **argv )
{
    char      optChar;
    char    * userc     = NULL;
    char    * pwc       = NULL;
    char    * groupc    = NULL;
    char    * methodc   = NULL;
    char    * cfgc      = NULL;
    bool      deluser   = false;
    bool      listusers = false;
    bool      showint   = false;
    bool      agent     = false;


    std::string   user, pw, group, method, config;

    while ( (optChar = getopt(argc, argv, "Ac:Dg:hiLp:m:u:V")) != EOF ) {
        switch ( optChar ) {
            case 'A':
                agent = true;
                break;
            case 'c':
            	cfgc = strdup(optarg);
            	break;
            case 'D':
                deluser = true;
                break;
            case 'u':
                userc = strdup(optarg);
                break;
            case 'p':
                pwc = strdup(optarg);
                break;
            case 'g':
                groupc = strdup(optarg);
                break;
            case 'i':
                showint = true;
                break;
            case 'L':
                listusers = true;
                break;
            case 'm':
                methodc = strdup(optarg);
                break;
            case 'V':
                version();
                break;
            default:
                usage();
                break;
        }
    }

    if ( userc ) {
        user = userc;
        ::free(userc);
    }

    if ( groupc ) {
        group = groupc;
        ::free(groupc);
    }

    if ( pwc ) {
        pw = pwc;
        ::free(pwc);
    }

    if ( cfgc ) {
    	config = cfgc;
    	::free(cfgc);
    }

    if ( methodc ) {
        method = methodc;
        ::free(methodc);
    }

    if ( user.empty() && ! listusers )
        usage();
    else if ( group.empty() && (! deluser && ! listusers) )
        usage();


    SqlSession * sql  = NULL;
    uint32_t     auid = 1;
    uint32_t     gid  = 0;
    uint32_t     uid  = 0;

    try {
        sql = new SqlSession(_Dbname, _Dbhost, _Dbuser, _Dbpass);
    } catch ( SqlException & err ) {
        std::cout << "Fatal Error initializing connection to database: "
            << err.toString() << std::endl;
        exit(1);
    }

    if ( ! sql->dbconnect() ) {
        std::cout << "Fatal error connecting to database: "
            << sql->sqlErrorStr() << std::endl;
        delete sql;
        exit(1);
    }

    if ( ! group.empty() )
        gid = getGID(sql, group);

    if ( listusers ) {
        listUsers(sql, gid);
        delete sql;
        exit(0);
    }

    if ( method.empty() )
        method = "dbstatic";

    auid = getAUID(sql, method);

    if ( auid == 0 ) {
        delete sql;
        exit(1);
    }

    uid = getUID(sql, user);

    if ( uid > 0 ) 
    {
        if ( deluser) {
            deleteUser(sql, user, uid);
        } else {
            if ( ! updateUser(sql, uid, gid, auid, pw, method) )
                uid = 0;
        }
    }
    else 
    {
        if ( deluser ) {
            std::cout << "User '" << user << "' doesn't exist" << std::endl;
        } else {
            uid = addUser(sql, user, pw, gid, auid);
        }
    }

    if ( uid > 0 && ! deluser )
    {
        setUserAgent(sql, uid, agent);
        if ( ! config.empty() ) {
            XmlDocument  doc;
            if ( ! doc.readFile(config) ) {
                std::cout << " Error parsing xml config: " << config << std::endl;
                std::cout << doc.getErrorStr() << std::endl;
            } else {
                std::string xmlcfg = doc.NodeToString(doc.getRootNode());
                setUserConfig(sql, uid, xmlcfg);
            }
    }

    sql->dbclose();
    delete sql;

    return 0;

}





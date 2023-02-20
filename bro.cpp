#include<iostream>
#include<map>
#include <forward_list>
#include<string.h>
#include<unistd.h>


#ifdef _WIN32
#include<windows.h>
// and compile as g++ bro.cpp -o bro.exe -lws3_32
#endif

#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif


using namespace std;


class BroUtilities
{
private:
	BroUtilities(){

	}
public:
	static void loadMIMETypes(map<string,string> &mimeTypesMap)
	{
		FILE *file;
		file = fopen("bro-data/mime.types.txt","r");
		if (file==NULL)
		{
			 return;
		}
		char *mimeType;
		char *extension;
		char line[200];
		int x;
		while(1)
		{
			fgets(line,200,file);
			
			if(feof(file)) break;
			
			if(line[0]=='#') continue;
			x= strlen(line)-1;
			while(1) //logic to remove \r\n from the end of the string
			{
				if(line[x]=='\r' || line[x]=='\n')
				{
					line[x]='\0';
					x--;

				}else
				{
					break;
				}
			}
		// logic to remove \r\n ends here
		mimeType = &line[0];
		for (x=0;line[x]!='\t';x++)
		{
			//nothing
		}
		line[x]='\0';
		x++;

		while(line[x]=='\t')
		{
			x++;
		}

		while(1)
		{
			extension= &line[x];
			while(line[x]!='\0' && line[x]!=' ')
			{
				x++;
			}
			if(line[x]=='\0')
			{
				//add entry to map
				mimeTypesMap.insert(pair<string,string>(string(extension),string(mimeType)));
				break;
			}
			else 
			{
				// place \0 on xth index, add entry to map and increment value of x
				mimeTypesMap.insert(pair<string,string>(string(extension),string(mimeType)));
				x++;

			}
		}









		}

		fclose(file);






	}
};


class StringUtility
{
private:
	StringUtility()
	{

	}
public:
	static void toLowerCase(char *str)
	{
		if(str==NULL) return;
		for(;*str;str++)
		{
			if(*str>=65 && *str<=90)
			{
				*str +=32;
			}
		}
	}
};


class HttpErrorStatusUtility{
private:
	HttpErrorStatusUtility()
	{

	}
public:
	static void sendBadRequestError(int clientSocketDescriptor)
	{


	}
	static void sendHttpVersionNotSupportedError(int clientSocketDescriptor, char *httpVersion)
	{

	}
	static void sendNotFoundError(int clientSocketDescriptor,char *requestURI)
	{
		char content[1000];
		char header[200];
		char response[1200];
		sprintf(content,"<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>404 Error</title></head><body>Request Resource [%s] Not found</body></html>",requestURI);
		int contentLength = strlen(content);
		sprintf(header,"HTTP/1.1 404 NOT FOUND\r\nContent-Type:text/html\r\nContent-Length:%d\nConnection:close\r\n\r\n",contentLength);
		strcpy(response,header);
		strcat(response,content);
		send(clientSocketDescriptor,response,strlen(response),0);
	}
	static void sendMethodNotAllowedError(int clientSocketDescriptor,char *method, char *requestURI)
	{

	}




};

// Amit [The bro programmer]

class Validator{
private:
	Validator(){

	}
public:	
static bool isValidMIMEType(string &mimeType){
return true;
}

static bool isValidPath(string &path)
{
	return true;
}

static bool isValidURLFormat(string &url){
	return true;
}
};

class Error{
private:
	string error;
public: 
	Error (string error){
		this->error = error;
	}
bool hasError(){
return this->error.length()>0;
}
string getError()
{
return this->error;
}
};

class Request{

private:
	char *method,*requestURI,*httpVersion;
	public:
		Request(char *method,char *requestURI,char *httpVersion)
	{
		this->method = method;
		this->requestURI = requestURI;
		this->httpVersion = httpVersion;

	}
	friend class bro; 



};
class Response{
private:
	forward_list <string> content;
	unsigned long contentLength;
	forward_list<string>::iterator contentIterator;
	string contentType;

public:
	Response()
	{
		this->contentLength=0;
		this->contentIterator = this->content.before_begin();

	}
	~Response()
	{

	}
void setContentType(string contentType){
if(Validator::isValidMIMEType(contentType)){
	this->contentType = contentType;
}

}
Response & operator<<(string content)
{
	this->contentLength+=content.length();
	this->contentIterator = this->content.insert_after(this->contentIterator,content);
	return *this;
}
// so that it can acccess the forwardlist and iterator
friend class HttpResponseUtility;

}; 


class HttpResponseUtility
{
private:
	HttpResponseUtility(){}
public:
	static void sendResponse(int clientSocketDescriptor, Response response)
	{
		char header[200];
		int contentLength = response.contentLength;
		sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type:text/html\r\nContent-Length:%d\nConnection:close\r\n\r\n",contentLength);
		send(clientSocketDescriptor,header,strlen(header),0);
		auto contentIterator = response.content.begin();
		while(contentIterator!=response.content.end())
		{
			string str = *contentIterator;
			send(clientSocketDescriptor,str.c_str(),str.length(),0);
			++contentIterator;
 
		}

	}


};

enum __request_method__{__GET__,__POST__,__PUT__, __DELETE__, __CONNECT__,__TRACE__, __HEAD__, __OPTIONS__ };

typedef struct __url__mapping
{
__request_method__ requestMethod;
void (*mappedFunction)(Request &,Response &);
}URLMapping;

class Bro{
private:
	string staticResourcesFolder;
	map<string, URLMapping> urlMappings;
	map<string,string> mimeTypes;
public:
	Bro(){
		BroUtilities::loadMIMETypes(mimeTypes);
		if(mimeTypes.size()==0)
		{
			throw string("bro-data folder has been tampered with");
		} 

	}
	~Bro()
	{

	}
public:
void setStaticResourcesFolder(string staticResourcesFolder){
	if(Validator::isValidPath(staticResourcesFolder))
	{
			this->staticResourcesFolder = staticResourcesFolder;


	}
	else
	{
		//not yet decided
	}

}
//for get method we will need 2 params
// first is url pattern and second is a pointer to a function which can store address of the 
//lambda function
// and how do we declare pointer to a function
//step-1: put a star * and then name of pointer 
//step-2: mention the return type of that function // void here
//step-3: and then mention the signature i.e. params type of that function

void get(string url, void (*callback)(Request &, Response &) ){
if(Validator::isValidURLFormat(url))
{
//insert in map
	urlMappings.insert(pair<string,URLMapping>(url,{__GET__,callback}));
}


}

void listen(int portNumber, void (*callback)(Error &))
{

	// for windows - to initialize socket api
	#ifdef _WIN32
		WSADATA wsaData;
		WORD ver;
		ver = MAKEWORD(1,1);
		WSAStartup(ver,&wsaData);
	#endif


	int serverSocketDescriptor;
	char requestBuffer[4097];
	int requestLength;
	int x;

	serverSocketDescriptor = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(serverSocketDescriptor<0)
	{
		// socket didn't get created
		#ifdef _WIN32
			WSACleanup();
		#endif
		Error error("Unable to create error");
		callback(error);
		return;

	}
	struct sockaddr_in serverSocketInformation;
	serverSocketInformation.sin_family=AF_INET;
	serverSocketInformation.sin_port=htons(portNumber); // we need to convert it to network byte order
	// why htons and not htonl == because port number range fits within the range of short and we don't need long
	serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
	int successCode = bind(serverSocketDescriptor, (struct sockaddr *) &serverSocketInformation, sizeof(serverSocketInformation));
	if(successCode<0)
	{
		//failed
		close(serverSocketDescriptor);
		#ifdef _WIN32
			WSACleanup();
		#endif
		char a[101];
		sprintf(a,"Unable to bind socket to port: %d",portNumber);
		Error error(a);
		callback(error);
		return;
	}

	int queue_size;
	queue_size = 10;
	successCode = ::listen(serverSocketDescriptor, queue_size ); // we are talking about global listen
	if(successCode<0)
	{
		close(serverSocketDescriptor);
		#ifdef _WIN32
			WSACleanup();
		#endif

		Error error("Unable to accept client connections");
		callback(error);
		return;
	}
	Error error(""); // zero error just to do a callback and allow the client programmer to print the welcome/server start message 
	callback(error);

	struct sockaddr_in clientSocketInformation;
	#ifdef _WIN32
		int len;
	#endif
	#ifdef linux
		socklen_t len;
	#endif

	
	len = sizeof(clientSocketInformation);

	while(1)
	{
	

		int clientSocketDescriptor;
		clientSocketDescriptor = accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);

		if(clientSocketDescriptor<0)
		{
			//not yet decided
			Error error("client socket clientSocketDescriptor<0");
			callback(error);
		}

		requestLength = recv(clientSocketDescriptor, requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
		if(requestLength==0 || requestLength ==-1)
		{
		//not yet decided


			close(clientSocketDescriptor);
			continue;

		}

		requestBuffer[requestLength] = '\0';
		//code to parse the first line of the request starts here
		//first line should be REQUEST_METHOD_TYPE SPACE URI SPACE HTTP-VERSION \r\n
		int i;
		char *method,  *requestURI, *httpVersion;
		method = requestBuffer; 


		i=0;
		while(requestBuffer[i]!='\0' && requestBuffer[i]!=' ')
		{
			i++;
		}



		if(requestBuffer[i]=='\0')
		{
			//request not received according to the http protocol
			HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
			close(clientSocketDescriptor);
			continue;
		}
		//space me backslash 0 fasaya
		requestBuffer[i]='\0';

		//start of uri
		i++;



		// if(requestBuffer[i]=='\0' || requestBuffer[i]!=' ')
		// {

			

		// 	//more spaces than one
		// 	//request not received according to the http protocol
		// 	HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
		// 	close(clientSocketDescriptor);
		// 	continue;
		// }




		StringUtility::toLowerCase(method);
		if(!(strcmp(method,"get")==0 ||
		strcmp(method,"post")==0 ||
		strcmp(method,"put")==0 ||
		strcmp(method,"delete")==0 ||
		strcmp(method,"head")==0 ||
		strcmp(method,"options")==0 ||
		strcmp(method,"trace")==0 ||
		strcmp(method,"connect")==0))
		{
			//if the request type is not of any type which we support, then error
			HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
			close(clientSocketDescriptor);
			continue;
		}





		requestURI = requestBuffer+i;
		while(requestBuffer[i]!= '\0' && requestBuffer[i]!=' ')
		{
			i++;
		}
		if(requestBuffer[i]=='\0')
		{
			//request not received according to the http protocol
			HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
			close(clientSocketDescriptor);
			continue;
		}
		requestBuffer[i]='\0';



		i++;
		if(requestBuffer[i]=='\0' || requestBuffer[i]=='\0')
		{
			HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
			close(clientSocketDescriptor);
			continue;
		}
		httpVersion = requestBuffer+i;
		while(requestBuffer[i]!='\0' && requestBuffer[i]!='\r' && requestBuffer[i]!='\n')
		{
			i++;
		}
		if(requestBuffer[i]=='\0')
		{
			//again a bad request - since line should have termianted by \r\n or \n
			HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
			close(clientSocketDescriptor);
			continue;
		}
		// if there is a \r then there should be \n as well
		if(requestBuffer[i]=='\r' && requestBuffer[i+1]!='\n')
		{
			HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
			close(clientSocketDescriptor);
			continue;
		}
		if(requestBuffer[i]=='\r')
		{
			requestBuffer[i]='\0';
			//header starts after \r\n
			i=i+2;
			// i is now representing the starting point of header
		}
		else
		{
			//there is no \r but only \n
			requestBuffer[i]='\0';
			i=i+1;
		}
		StringUtility::toLowerCase(httpVersion);
		if(strcmp(httpVersion,"http/1.1")!=0)
		{
			HttpErrorStatusUtility::sendHttpVersionNotSupportedError(clientSocketDescriptor,httpVersion);
			close(clientSocketDescriptor);
			continue;
		}

		// we need to check if request uri exists in a map or not
		auto urlMappingsIterator = urlMappings.find(requestURI);
		if(urlMappingsIterator==urlMappings.end())
		{
			HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor,requestURI);
			close(clientSocketDescriptor);
			continue;
		}
		URLMapping urlMapping = urlMappingsIterator->second;
		if(urlMapping.requestMethod == __GET__ && strcmp(method,"get")!=0)
		{
			HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDescriptor,method,requestURI);
			close(clientSocketDescriptor);
			continue;
		}
		// code to parse header and payload (if exists) starts here


		// code to parse header and payload (if exists) end here
		Request request(method,requestURI,httpVersion);
		Response response;
		urlMapping.mappedFunction(request,response);
		HttpResponseUtility::sendResponse(clientSocketDescriptor,response);























		close(clientSocketDescriptor);


 




	} // infinite loop ends here
	#ifdef _WIN32
		WSACleanup();
	#endif


}

 
};







// Bobby [The app developer]

int main()
{
Bro bro;
// link to all static resources files eg. js , css files
bro.setStaticResourcesFolder("whatever");





// serve welcome html when a request arrives with "/"
bro.get("/",[] (Request &request, Response &response)   {
const char *html = R""""(   // multiline string - pointer named html will be pointing to the base address of this html
<!DOCTYPE HTML>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Whatever</title>
</head>
<body>
<h1>Welcome</h1>
<h3>Some text</h3>
<a href="getCustomers">Customers List</a>
</body>
</html>
)"""";
response.setContentType("text/html"); //setting mime type
response<<html; 
});


bro.get("/getCustomers",[](Request &request, Response &response) {
 
const char *html = R""""(
<!DOCTYPE HTML>
<html>
<head>
<meta charset="utf-8">
<title>Whatever</title>
</head>
<body>
<h1>List of customers</h1>
<ul>
<li>Ramesh</li>
<li>Suresh</li>
<li>Mohan</li>
</ul>
<a href="/">Home</a>

</body>
</html>

)"""";
response.setContentType("text/html"); //setting mime type
response<<html;

}); 

bro.listen(6060,[](Error &error) {
//if server is able to bind itself at this port number this lambda expression will get called with no errors
if(error.hasError()){
cout<<error.getError();
return;
}
cout<<"Bro HTTP Server is ready to accept request on port 6060"<<endl;



});


return 0;
}
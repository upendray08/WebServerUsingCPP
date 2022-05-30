#include "webserver.h"
#include "Socket.h"
#include "bits/stdc++.h"
#include "iostream"
#include "string"
#include "sstream"
#include "fstream"

using namespace std;

#include<bits/stdc++.h>
using namespace std;
class Stock{
    public:
    priority_queue<pair<float, float> > buy; //max heap
    priority_queue <pair<float,float>, vector<pair<float,float>>, greater<pair<float,float>> > sell;
   
    void addpriceinbuy(float price,float noofstock){
        buy.push({price,noofstock});
    }
    void addpriceinsell(float price,float noofstock){
        sell.push({price,noofstock});
    }
    void display(){
        while (!buy.empty()) {
        cout << buy.top().first
             << " " << buy.top().second
             << endl;
        buy.pop();
    }
    cout << endl;
    }
};

int verification(vector<string> input){
  string stockname = input[0];
  string price = input[1];
  float price_int=atof(input[1].c_str());
  string numOfStocks=input[2];
  float  num_int=atof(input[2].c_str());
  //verification part
  
  if(price_int <=0.0 || num_int<=0.0  ){
    return false;
  }
  return true;

}

void Request_Handler(webserver::http_request* r) {
  Stock axis;
  Stock icici;
  Socket s = *(r->s_);

  std::string title;
  std::string body;
  std::string bgcolor="#ffffff";
  std::string links =
      "<p><a href='/red'>red</a> "
      "<br><a href='/blue'>blue</a> "
      "<br><a href='/form'>form</a> "
      "<br><a href='/auth'>authentication example</a> [use <b>rene</b> as username and <b>secretGarden</b> as password"
      "<br><a href='/header'>show some HTTP header details</a> "
      ;

  if(r->path_ == "/") {
    title = "Web Server Example";
    body  = "<h1>Welcome to Our Web Server</h1>"
            "Let's start with today's Hackathon"  + links;
  }
  else if (r->path_ == "/red") {
    bgcolor = "#ff4444";
    title   = "You chose red";
    body    = "<h1>Red</h1>" + links;
  }
  else if (r->path_ == "/blue") {
    bgcolor = "#4444ff";
    title   = "You chose blue";
    body    = "<h1>Blue</h1>" + links;
  }
  else if (r->path_ == "/form") {
    title   = "buy";

    body    = "<h1>BUY</h1>";
    body   += "<form action='/form'>"
              "<table>"
              "<tr><td>STOCK NAME</td><td><input name=field_1></td></tr>"
              "<tr><td>BUY PRICE</td><td><input name=field_2></td></tr>"
              "<tr><td>NUMBER OF STOCKS/td><td><input name=field_3></td></tr>"
              "</table>"
              "<input type=submit></form>";

    vector<string> input;
     for (std::map<std::string, std::string>::const_iterator i = r->params_.begin();
          i != r->params_.end();
          i++) {
        input.push_back(std::string (i->second));
       body += "<br>" + i->first + " = " + i->second + "<br>"  ;

    }
      string price = input[1];
      float price_int=atof(input[1].c_str());
      string numOfStocks=input[2];
      float  num_int=atof(input[2].c_str());
     
     //verification part 
      bool verify;
      verify=verification(input);
    if(!verify){
       body+="<br>"
      "the input is invalid";
    }

     //
    else if(verify ){
      string stockName =input[0];
      if(stockName=="axis"){
        //the sell section is empty
        if(axis.sell.empty()){
          axis.addpriceinbuy(price_int,num_int);
        }
        //first condition:the price buy price is lower than the top selling price 
        else if(axis.sell.top().first> price_int){
          axis.addpriceinbuy(price_int,num_int);
        }
        //there is a hit 
        else if( axis.sell.top().first<=price_int){
          int noofstocksToSell=max(axis.sell.top().second,num_int);
          while(noofstocksToSell>0){
            if(axis.sell.empty() || axis.sell.top().first>price_int){
              axis.addpriceinbuy(price_int,noofstocksToSell);
              break;
            }
            int top=axis.sell.top().second;
            if(top<=num_int){
              noofstocksToSell-=top;
              axis.sell.pop();
            }
          }
        }
        //data is not stored for ec=ach entry as the page is reloaded after 
        axis.display();
        fstream myfile;

        myfile.open("order.txt", ios::app);
        myfile<<price_int<<"\t\t"<<num_int<<endl;
      }
      if(stockName=="icici"){
        //the sell section is empty
        if(icici.sell.empty()){
          icici.addpriceinbuy(price_int,num_int);
        }
        //first condition:the price buy price is lower than the top selling price 
        else if(icici.sell.top().first> price_int){
          icici.addpriceinbuy(price_int,num_int);
        }
        //there is a hit 
        else if(icici.sell.top().first<=price_int){
          int noofstocksToSell=max(icici.sell.top().second,num_int);
          while(noofstocksToSell>0){
            if(icici.sell.empty() || icici.sell.top().first>price_int){
              icici.addpriceinbuy(price_int,noofstocksToSell);
              break;
            }
            int top=icici.sell.top().second;
            if(top<=num_int){
              noofstocksToSell-=top;
              icici.sell.pop();
            }
          }
        }
        //data is not stored for ec=ach entry as the page is reloaded after 
        icici.display();

      }
    }

    body += "<hr>" + links;

  }
  else if (r->path_ == "/auth") {
    if (r->authentication_given_) {
      if (r->username_ == "rene" && r->password_ == "secretGarden") {
         body = "<h1>Successfully authenticated</h1>" + links;
      }
      else {
         body = "<h1>Wrong username or password</h1>" + links;
         r->auth_realm_ = "Private Stuff";
      }
    }
    else {
      r->auth_realm_ = "Private Stuff";
    }
  }
  else if (r->path_ == "/header") {
    title   = "some HTTP header details";
    body    = std::string ("<table>")                                   +
              "<tr><td>Accept:</td><td>"          + r->accept_          + "</td></tr>" +
              "<tr><td>Accept-Encoding:</td><td>" + r->accept_encoding_ + "</td></tr>" +
              "<tr><td>Accept-Language:</td><td>" + r->accept_language_ + "</td></tr>" +
              "<tr><td>User-Agent:</td><td>"      + r->user_agent_      + "</td></tr>" +
              "</table>"                                                +
              links;
  }
  else {
    r->status_ = "404 Not Found";
    title      = "Wrong URL";
    body       = "<h1>Wrong URL</h1>";
    body      += "Path is : &gt;" + r->path_ + "&lt;"; 
  }

  r->answer_  = "<html><head><title>";
  r->answer_ += title;
  r->answer_ += "</title></head><body bgcolor='" + bgcolor + "'>";
  r->answer_ += body;
  r->answer_ += "</body></html>";
}

int main() {
  webserver(4500, Request_Handler);
}

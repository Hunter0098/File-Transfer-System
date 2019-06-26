#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>     //The header file in.h contains constants and structures needed for internet domain addresses
#include <unistd.h>     //For read
#include <fstream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <exception>
#include <sys/stat.h>
using namespace std;

mutex mtx, mtx2;
float bytesSent=0;

int server(string filepath)
{
    sleep(1);
    mtx.lock();
    cout<<"Server started!"<<endl;
    char buffer[100],buffer2[1024];
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    listen(sockfd , 2);

    int connection1 = accept(sockfd , (struct sockaddr *)&address, (socklen_t*)&addrlen);
    read(connection1, buffer, 100);
    cout<<buffer<<endl;
    ifstream fobject;
    fobject.open(filepath, ios::binary);
    while(!fobject.eof())
    {
        fobject.read(buffer2, sizeof(buffer2));
        write(connection1, buffer2, sizeof(buffer2));
        mtx2.lock();
        bytesSent+= strlen(buffer2);
        mtx2.unlock();
        cout<<sizeof(buffer2)<<endl;
    }
    fobject.close();
    close(sockfd);
    mtx.unlock();
    return 0;
}

int gui(string filepath)
{
    mtx.lock();
    float filesize=0;
    ifstream fobject;
    fobject.open(filepath, ios::binary);
    long start = fobject.tellg();
    fobject.seekg(0, ios::end);
    long end = fobject.tellg();
    filesize = end-start;
    fobject.close();
    cout<<filesize<<endl;

    sf::RenderWindow window(sf::VideoMode(600,500), "File Transfer System");
    window.setFramerateLimit(60);

    int i;
    sf::Text txt1;
    vector<sf::Text>vec;
    sf::Font font;
    font.loadFromFile("FiraSans-Book.otf");
    sf::String str1;
    string textString = "Ready to send";
    txt1.setFont(font);
    for(i=0; i<textString.length(); i++)
    {
        str1 = textString[i];
        txt1.setString(str1);
        txt1.setFillColor(sf::Color::Magenta);
        txt1.setPosition(sf::Vector2f(20+(17*i),-50));
        vec.push_back(txt1);
    }
    i=0;

    sf::RectangleShape button(sf::Vector2f(100,50));
    button.setFillColor(sf::Color::Blue);
    button.setPosition(sf::Vector2f(250,350));

    sf::RectangleShape buttonActive(sf::Vector2f(100,0));
    buttonActive.setFillColor(sf::Color::Cyan);
    buttonActive.setPosition(sf::Vector2f(250,350));

    sf::Mouse mouse;

    sf::Text txt2;
    sf::String str2;
    str2 = "Send";
    txt2.setFont(font);
    txt2.setString(str2);
    txt2.setPosition(sf::Vector2f(button.getPosition().x+12, button.getPosition().y+5));
    txt2.setFillColor(sf::Color::White);

    sf::Text txt3;
    sf::String str3;
    string path;
    int count;
    for(int i=0; i<filepath.length(); i++)
    {
        if(count == 52)
        {
            path+="\n";
            count=0;
        }
        count++;
        path+=filepath[i];
    }
    str3 = "File path:\n" + path;
    txt3.setFont(font);
    txt3.setString(str3);
    txt3.setPosition(20,100);
    txt3.setFillColor(sf::Color::Blue);
    txt3.setCharacterSize(20);

    sf::Text txt4;
    sf::String str4;
    str4 = "Size: " + to_string(filesize) + " bytes";
    txt4.setFont(font);
    txt4.setString(str4);
    txt4.setPosition(sf::Vector2f(20, 180));
    txt4.setCharacterSize(20);
    txt4.setFillColor(sf::Color::Blue);

    sf::Text txt5;
    sf::String str5;
    str5 = "Progress:";
    txt5.setFont(font);
    txt5.setString(str5);
    txt5.setPosition(sf::Vector2f(20, 260));
    txt5.setCharacterSize(20);
    txt5.setFillColor(sf::Color::Blue);

    sf::RectangleShape progressbarBase(sf::Vector2f(550,20));
    progressbarBase.setFillColor(sf::Color::Yellow);
    progressbarBase.setPosition(sf::Vector2f(20,300));

    sf::RectangleShape progressbarActive(sf::Vector2f(0,20));
    progressbarActive.setFillColor(sf::Color::Green);
    progressbarActive.setPosition(sf::Vector2f(20,300));

    i=0;
    sf::Event event;
    while(window.isOpen())
    {
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    window.close();
            }
        }

        if(vec[i].getPosition().y!= 10)
        {
            vec[i].move(0,4);
            sleep(0.1);
        }
        else if(i < textString.length())
        {
            i++;
        }

        //cout<<mouse.getPosition(window).x<<" "<<mouse.getPosition(window).y<<endl;
        if(mouse.getPosition(window).x > button.getPosition().x && mouse.getPosition(window).x < button.getPosition().x+button.getSize().x && mouse.getPosition(window).y > button.getPosition().y && mouse.getPosition(window).y < button.getPosition().y+button.getSize().y && buttonActive.getSize().y < button.getSize().y+1)
        {
            buttonActive.setSize(sf::Vector2f(buttonActive.getSize().x,buttonActive.getSize().y+1));
            if(mouse.isButtonPressed(sf::Mouse::Button::Left))
            {
                mtx.unlock();
            }
        }
        else if(buttonActive.getSize().y > 0)
        {
            buttonActive.setSize(sf::Vector2f(buttonActive.getSize().x,buttonActive.getSize().y-1));
        }

        mtx2.lock();
        int onepercent = progressbarBase.getSize().x/100;
        float progressPercentage = (bytesSent/filesize)*100;
        //cout<<"BytesSent: "<<bytesSent<<" ProgressPercentage: "<<progressPercentage<<endl;
        progressbarActive.setSize(sf::Vector2f(onepercent*progressPercentage, progressbarActive.getSize().y));
        mtx2.unlock();

        window.clear(sf::Color::White);
        for(int j=0; j<textString.length(); j++)
        {
            window.draw(vec[j]);
        }
        window.draw(button);
        window.draw(buttonActive);
        window.draw(txt2);
        window.draw(txt3);
        window.draw(txt4);
        window.draw(txt5);
        window.draw(progressbarBase);
        window.draw(progressbarActive);
        window.display();
    }
    return 0;
}

int main()
{
    string filepath = "/home/vaibhav/Videos/Snowden (2016) [1080p] [YTS.AG]/Snowden.2016.1080p.BluRay.x264-[YTS.AG].mp4";
    thread t1(gui, filepath);
    thread t2(server, filepath);
    t1.join();
    //! Fix needed: t2.join();
    return 0;
}
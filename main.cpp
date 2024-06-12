#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Network/TcpSocket.hpp>
using namespace std;
using namespace sf;
#define ll long long
long long SmallGen(ll x, ll y, ll seed){
    double res;
    res = (x*seed*seed)^y*seed;
    return round(res);
}
double PerlinNoise(ll x, ll y, ll seed){
    double res, n, genp;
    ll gen;
    gen = SmallGen(x, y, seed);
    if(x==0)x=1;
    if(y==0)y=1;
    n=gen^(x*(-1*y));
    n /= (x+y*y);
    if(n==1.0/0.0)return PerlinNoise(abs(x),-y,gen);
    while(n*10<1.0)n=abs(n*10)+1;
    while(n>1.0)n=abs(n/10)-abs(n);

    //cout << "n=" << n << "\tx=" << x << "\ty=" << y << endl;
    return n;
}
ll x_wind=300, y_wind=300, seed=52;
int main()
{
    RenderWindow window(VideoMode(x_wind, y_wind), "Perlin test");
    window.clear();
    ll x=0, y=0, step=6;
    double maxi=0,maxi_col=0;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        double col = PerlinNoise(x, y, seed);
        maxi+=col;
        maxi_col+=1;
        col *= 255.0;
        CircleShape shape(step);
        shape.setFillColor(Color(col,col,col));
        shape.setPosition(Vector2f(x, y));
        if(x<x_wind){
            x+=step;
        }else if(y<y_wind){
            y+=step;
            x=0;
        }
        if(y<y_wind){
            window.draw(shape);
            window.display();
        }
    }
    cout<<"====\n"<<maxi/maxi_col;
    return 0;
}

#include <iostream>
#include <vector>
#include <type_traits>
using namespace std;

class test{
public:
    int i = 0;
    int total = 0;

    int testo(int par, float mi);
    int func();
};

int test::testo(int par, float mi){
    vector<float> list;
    int n;
    decltype(list.size()) vB;
    decltype(i < n) vQ;
    std::remove_reference_t<decltype(list[i])> Ni;
    decltype(i + 1) NU;

L0:
    list = vector<float>(14);
    list[0] = 5.0;
    list[1] = 6.0;
    list[2] = 1.0;
    list[3] = 2.4;
    list[4] = 2.3;
    list[5] = 2.5;
    list[6] = 9.0;
    list[7] = 14.0;
    list[8] = 2.0;
    list[9] = 15.0;
    list[10] = 6.0;
    list[11] = 7.0;
    list[12] = 8.0;
    list[13] = 97.0;
    vB = list.size();
    n = vB;
    i = 0;
    i = 0;
    goto L1;

L1:
    vQ = i < n;
    if(vQ) goto L2;
    goto L3;

L2:
    Ni = list[i];
    cout << Ni << endl;
    NU = i + 1;
    i = NU;
    goto L1;

L3:
    return 0;

}

int test::func(){
    int n;
    int x;
    decltype(i <= n) wv;
    decltype(x * i) UT;
    decltype(i + 1) jv;
    decltype(x < 3) SY;

L0:
    cin >> n;
    x = 1;
    i = 2;
    i = 2;
    goto L1;

L1:
    wv = i <= n;
    if(wv) goto L2;
    goto L3;

L2:
    UT = x * i;
    x = UT;
    jv = i + 1;
    i = jv;
    goto L1;

L3:
    goto L4;

L4:
    SY = x < 3;
    if(SY) goto L5;
    goto L6;

L5:
    x = 0;
    goto L6;

L6:
    cout << x << endl;
    return 0;

}

int main(){
    test object;
    int n;
    decltype(object.testo(4, 5.4)) wn;
    decltype(n < 3) Yk;
    int x;
    int i;

L0:
    wn = object.testo(4, 5.4);
    n = wn;
    cin >> n;
    goto L1;

L1:
    Yk = n < 3;
    if(Yk) goto L2;
    goto L3;

L2:
    return 0;
    goto L3;

L3:
    x = 1;
    i = 2;
    return 0;

}


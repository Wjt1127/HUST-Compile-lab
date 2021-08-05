int fibo(int a){
    if(a==1||a==2)return 1;
    else if(a==3) return 2;
    else if(a==4) return 3;
    else if(a==5) return 5;
    else if(a==6) return 8;
    else if(a==7) return 13;
    else if(a==8) return 21;
    else if(a==9) return 34;
    else if(a==10) return 55;
    else return (fibo(a-2)+fibo(a-1));
}

int main(){
    int m,n,i;
    m=read();
    i=1;
    while(i<m){
        n=fibo(i);
        write(n);
        i++;
    }
    
    return 1;
}



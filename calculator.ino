#include <Keypad.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,8,9,10,13);
const byte ROWS = 4,COLS = 4; char key;int x=0,y=0;char keys[ROWS][COLS] = {{'1','2','3','+'},{'4','5','6','-' },{'7','8','9','/'},{'*','0','#','='}};
byte rowPins[ROWS] = {A0,A1,A2,A3}; byte colPins[COLS] = {A4,A5,5,6};
int arr[5][32],count=0;
int col[15],a[32],b[32],c[32];char op;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  reset();
}
void reset()
{
  int i,j; 
  for(i=0;i<5;i++)
    for(j=0;j<32;j++)
      arr[i][j]=0;
   for(i=0;i<15;i++)
    col[i]=0;
   for(i=0;i<32;i++){a[i]=0;b[i]=0;c[i]=0;}
   x=0,y=0;
   count=0;
}
void loop(){
  
  key = keypad.getKey();
  if (key != NO_KEY)
  {    
    if(count%16==0&&count!=0)
      lcd.setCursor(0,1);
    if(count%32==0)
    {
      lcd.setCursor(0,0);
      lcd.clear();
    }
    count++;
    switch(key)
    {
      case '#':lcd.clear();count=0;reset();break;
      case '+':lcd.print("+");op='+';col[x]=y;x++;y=0;break;
      case '-':lcd.print("-");op='-';col[x]=y;x++;y=0;break;
      case '*':lcd.print("*");op='*';col[x]=y;x++;y=0;break;
      case '/':lcd.print("/");op='/';col[x]=y;x++;y=0;break;
      case '=':col[x]=y;x++;lcd.print(key);
              switch(op)
              {
                case '+':add();break;
                case '-':sub();break;
                case '*':mult();break; 
                case '/':div();break;
              }
              break;
      default:arr[x][y++]=key-48;lcd.print(key);
    }
  }
}
void sub()
{
  int i,j,k,carry=0;
  int l1=col[0]-1,l2=col[1]-1;
  for(i=0;i<col[0];i++)
    a[i]=arr[0][i];
  for(i=0;i<col[1];i++)
    b[i]=arr[1][i];
  int max=compare(l1,l2);
  int ct=0,mx=l1>l2?l1+1:l2+1;
  if(max==0)
  {
    for(i=0;i<32;i++){a[i]=0;b[i]=0;}
    for(i=0;i<col[0];i++)
      b[i]=arr[0][i];
    for(i=0;i<col[1];i++)
      a[i]=arr[1][i];
    l1=col[1];
    l2=col[0];
  }
  /*if(32-count<=mx)
  {*/
    lcd.clear();
    lcd.setCursor(0,0);
  //}
  int as=max==0?l2:l1;
  for(i=0;i<=as;i++)
  {
    if(a[l1]<b[l2])
    {
      carrying(l1,l2);
    }
    if(l2<0)
    {
      c[i]=a[l1--];
    }
    if(l2>=0)
    {
      c[i]=a[l1--]-b[l2--];
    }
  }
  char ch='-';
  int low=max==0?1:0;
  if(max==0)
  {
    //Serial.print(ch);
    lcd.print(ch);
    ct++;
  }
  if(max==0&&l1-l2>0)
  {
    if(ct%16==0&&ct!=0)
      lcd.setCursor(0,1);
    ct++;
    for(i=0;i<l1-l2;i++)
      lcd.print(a[i]);
      //Serial.print(a[i]);
  }
  for(i=as;i>=low;i--)
  {
    if(ct%16==0&&ct!=0)
      lcd.setCursor(0,1);
    ct++;
    lcd.print(c[i]);
    //Serial.print(c[i]);
  } 
  reset();
}
void carrying(int l1,int l2)
{
  int i,j;
  for(i=l1-1;i>=0;i--)
  {
    if(a[i]!=0)
    {
      a[i]--;
      a[i+1]=10+a[i+1];
      for(j=i+2;j<=l1;j++)
      {
        a[j-1]--;
        a[j]=10+a[j];
      }
      break;
    }
  }
}
int compare(int l1,int l2){
  if(l1>l2)
    return 1;
  if(l1<l2)
    return 0;
  int i=0;
  while(true){
    if(a[i]>b[i])
      return 1;
    if(a[i]<b[i])
      return 0;
    i++;
  }
}
void mult()
{
  int i,j,ct=0,l1=col[0]-1,l2=col[1]-1,carry=0,s[32];
  //if(l1+l2+2>=16){
    lcd.clear();
    lcd.setCursor(0,0);//}
  //else
    ct=0;
  for(i=0;i<col[0];i++)
    a[i]=arr[0][i];
  for(i=0;i<32;i++) {c[i]=0;s[i]=0;}
  for(i=0;i<col[1];i++)
    b[i]=arr[1][i];
  for(i=l2;i>=0;i--)
  {
    s[i+1]=carry;
    carry=0;
    for(j=l1;j>=0;j--)
    {
      c[i+j+1]=c[i+j+1]+a[j]*b[i]+carry;
      carry=c[i+j+1]/10;
      c[i+j+1]%=10;
    }
  }
  c[0]=carry;carry=0;
  for(i=l1+l2+1;i>=0;i--){
    int sum=c[i]+s[i]+carry;
    a[i]=sum%10;
    carry=sum/10;
  }
  int low=0;
  for(low=0;low<10;low++)
    if(a[low]!=0)
      break;
  for(i=low;i<=l1+l2+1;i++)
  {
    if(ct%16==0&&ct!=0)
      lcd.setCursor(0,1);
    ct++;
    Serial.print(a[i]);
    lcd.print(a[i]);
  }
  reset();
}

void div()
{
  int i,j,k,carry=0,ct=0;
  long long num1=0,num2=0,num3=0,num4=0;
  int l1=col[0]-1,l2=col[1]-1;
  lcd.clear();
  lcd.setCursor(0,0);
  for(i=col[0]-1;i>=0;i--)    a[i]=arr[0][i];
  for(i=col[1]-1;i>=0;i--)    b[i]=arr[1][i];
  for(i=0;i<col[0];i++)    num1=a[i]+num1*10;
  for(i=0;i<col[1];i++)    num2=b[i]+num2*10;
  num3=num1/num2;
  if(num3==0){
    lcd.print("0");}
    //Serial.print("0");
  while(num3!=0)
  {
    int ab=num3%10;
    num4=ab+num4*10;
    num3=num3/10;
  }
  while(num4!=0)
  {
    if(ct%16==0&&ct!=0)
      lcd.setCursor(0,1);
    ct++;
    int ab=num4%10;
    Serial.print(ab);
    lcd.print(ab);
    num4=num4/10;
  }
  reset();
}
void add()
{
  int i,j,k,carry=0;
  int l1=col[0]-1,l2=col[1]-1;
  for(i=0;i<32;i++){a[i]=0;b[i]=0;}
  for(i=0;i<col[0];i++)
    a[i]=arr[0][i];
  for(i=0;i<col[1];i++)
    b[i]=arr[1][i];
  int max=col[0]>col[1]?col[0]:col[1];
  for(i=max-1;i>=0;i--)
  {
    int aa=a[l1--],bb=b[l2--];
    if(l1+1<0) aa=0;
    if(l2+1<0) bb=0; 
    int sum=aa+bb+carry;
    c[i]=sum%10;
    carry=sum/10;
  }
  if(32-count<=max)
  {
    lcd.clear();
    lcd.setCursor(0,0);
  }
  int ct=0;
  if(carry!=0)
  {
    lcd.print(carry);
    ct++;
  }
    
  for(i=0;i<max;i++)
  {
    if(ct%16==0&&ct!=0)
      lcd.setCursor(0,1);
    ct++;
    lcd.print(c[i]);
  }    
  reset();
}

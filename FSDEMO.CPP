
#include<graphics.h>
#include <fstream.h>
#include <conio.h>
#include<iostream.h>
#include <string.h>
#include <stdlib.h>
#include<dos.h>
#include<stdio.h>
#include<ctype.h>
#include<iomanip.h>
#define datafile "datafile3.txt"
#define indexfile "index5new.txt"
fstream stdfile,indfile;
int i, indsize;
int checkbid(char bid[])
{
	if(strlen(bid)==6)
		return 1;
	else
		return 0;
}
class block
{
	  public:
		 char keys[4][11];
		 block *link;
		 int disp[4];
		 int cnt;
		 block();
		 ~block(){}
		 void split();
		 void merge();
};
class SSET
{
	  public:
			typedef block *BK;
		 SSET();
		 ~SSET(){}
		 void create();
		 int Insert(char*,int);
		 void del(char*);
		 void update();
		 block *FindNode(char*);
		 void display();

	 };

class node	// class for btree node
 {
	  public:
		 char keys[4][11];
		 node *dptrs[4];
		 node *uptr;
		 block *ptr[4];
			int cnt;
		 node();
		 ~node(){}
			int isLeaf();
		 void split(node *,node *,node *);
	 };
class btree
	 {
	  public:
		 btree();
		 int insert(char*,block *);
		 node* findLeaf(char*,int&);
		 block *search(char*,int&);
		 void create();
		 void dis(node*);
		 ~btree(){}
		 node *root;
	 };
class person
	 {
	  public:
		 char bid[10];
		 char Name[10];
		 char num_seats[10];
		 char tot_cost[10];
		 char theatre_no[15];
		 char City[10];
		 char Payment[10];
		 char th_name[10];
		 void Clear();
		 void unpack1();
		 void dataDisp();
		 int Unpack(fstream&);
		 int Pack(fstream&);
		 void Input(int);
		 void Display();
		 ~person(){}
		 void Assign(person&);
	 };
class index
{
	public:
		char bid[15],addr[5];
		void initial();
		void write();
}in,id[100];
void index::initial()
{
	indfile.open(indexfile,ios::in);
	if(!indfile)
	{
		indsize=0;
		return;
	}
	for(indsize=0;;indsize++)
	{
		indfile.getline(id[indsize].bid,15,'|');
		indfile.getline(id[indsize].addr,5,'\n');
		if(indfile.eof())
			break;
	}
	indfile.close();
}
void opener(fstream &sfile,char* fn,int mode)
{
	sfile.open(fn,mode);
	if(!sfile)
	{
		cout<<"Unable to open the file\n";
		exit(1);
	}
}
void index::write()
{
	opener(indfile,indexfile,ios::out);
	for(i=0;i<indsize;i++)
		indfile<<id[i].bid<<"|"<<id[i].addr<<"\n";
	indfile.close();
}
class iobuf
	 {
		 protected:
		 char Buffer[100];
		 int BufSiz;
		 public:
		 void Clear();
		 virtual int Read(fstream&)=0;
		 virtual int Write(fstream&)=0;
		 virtual int Pack(person&,fstream&)=0;
		 virtual int Unpack(person&,fstream&)=0;
	 };
class varlen : public iobuf
	 {
	  public:
		 int Read(fstream&);
		 int Write(fstream&);
	 };
class delmfld : public varlen
	 {
		 public:
		 int Pack(person&,fstream&);
		 int Unpack(person&,fstream&);
		};
btree bt;
SSET s;
node *dummy,*dummy1;
block *head;
static int tot,usd;
int indsearch(char* bid)
{
	int low=0,high=indsize-1;
	int mid;
	while(low<=high)
	{
		mid=(low+high)/2;
		if(strcmp(bid,id[mid].bid)==0)
			return mid;
		else if(strcmp(bid,id[mid].bid)>0)
			low=mid+1;
		else
			high=mid-1;
	}
	return -1;
}
void update(char *key)
{
 person stds[100],std;
 int f=0,found=0,g;
char upd;
 fstream file(datafile,ios::in);
 file.seekg(0,ios::beg);
 while(!file.fail())
  if(std.Unpack(file))
	if(strcmpi(std.bid,key)==0)
   {
    found=1;
	 cout<<"\n\tRecord:";
    std.Display();
    cout<<"\n\n Confirm permanent updation:[Y/N] ";
    cin>>upd;
	if(upd == 'n' || upd == 'N')
		return;
    else
    {
	cout << "\n\t Enter the new record :\n";
	std.Input(1);
	strcpy(std.bid,key);
	stds[f].Clear();
	stds[f++].Assign(std);
    }
	}
   file.clear();
   if(found==0)
    cout<<"\n\n\t Record not found...!\n";
   else
   {
    file.close();
    file.open(datafile,ios::out);
    file.seekp(0,ios::beg);
    for(g=0;g<f;g++)
	if(!(stds[g].Pack(file))) continue;
    file.clear();
   }
   file.close();
   delete head;
	delete bt.root;
   head = new block;
   bt.root = new node;
	s.create();
}
void search(char *key)
{
  person std;
  int found=0,i;
  block *dp;
  fstream file(datafile,ios::in);
  file.seekg(ios::beg);
  dp=bt.search(key,found);
  if(found==0)
	cout<<"\n\n\t Record not found...!\n";
  else
  {
	 found=0;
	 for(i=0;i<dp->cnt;i++)
	  if(strcmpi(key,dp->keys[i])==0)
	  {
		found = 1;
		file.seekg(dp->disp[i],ios::beg);
	 std.Unpack(file);
	 cout<<"\n\n\t Record found : ";
	 std.Display();
	}
    if(found==0) cout<<"\n\n\t Record not found ";
  }
  file.clear();
  file.close();
}
void append()
{
  person std;
  int flag=1, pos,k;
  fstream file(datafile,ios::app);
  std.Input(0);
	for(i=indsize;i>0;i--)
	{
		if(strcmp(std.bid,id[i-1].bid)<0)
			id[i]=id[i-1];
		else
			break;
	}
  file.seekp(0,ios::end);
  pos=file.tellp();
  k=file.tellg();
  flag=s.Insert(std.bid,pos);
	strcpy(id[i].bid,std.bid);
	itoa(k,id[i].addr,10);
	indsize++;
  if(flag && std.Pack(file))
  { cout << "\n\t Done...\n";
  }
  else cout << "\n\t Failure.";
  file.clear();
  file.close();
}
void delrec(char *key)
{
  int r=0,found=0,s;
  char del='N';
  person stds[100],std;
  fstream file(datafile,ios::in);
  file.seekg(0,ios::beg);

  while(!file.fail())
    if(std.Unpack(file))
	 if(strcmpi(std.bid,key)==0)
	 {
	found=1;
	cout<<" \n Record :";
	std.Display();
	cout<<"\n\n Confirm permanent deletion:[Y/N]";
	cin>>del;
	if(del == 'n' || del == 'N')
		return;
	if(!(del=='Y' || del=='y'))
	{
	  stds[r].Clear();
	  stds[r++].Assign(std);
	}
	else
	cout<<"\n\n\t Deleted : \n\n";
	 }
	 else
	 {
	stds[r].Clear();
	stds[r++].Assign(std);
	 }
  file.clear();
  if(found==0) cout<<"\n\n\t Record not found.";
  else
  {
    file.close();
    file.open(datafile,ios::out);
    file.seekp(0,ios::beg);
    for(s=0;s<r;s++)
	 if(!(stds[s].Pack(file))) continue;
    file.clear();
  }
  file.close();
}

void driver(node *p)
{
  fstream fl;
  int i,j;
  char a[3]={'\0','\0','\0'};
  char st[1]={'\0'};
  char st1[1]={'\0'};
  char *tmp;
  person std;
  strcpy(st,"|");
  strcpy(st1,"#");
  if(p->isLeaf())
  {
    for(i=0;i<p->cnt;i++)
    {
	block *t=p->ptr[i];
	while(strlen(tmp))
	 strcpy(tmp,"");
	for(j=0;j<t->cnt-1;j++)
	{
	 itoa(t->disp[j],a,10);
	 strcat(tmp,t->keys[j]);
	 strcat(tmp,st);
	 strcat(tmp,a);
	 strcat(tmp,st);
	}
	itoa(t->disp[j],a,10);
	strcat(tmp,t->keys[j]);
	strcat(tmp,st);
	  strcat(tmp,a);
	strcat(tmp,st1);
	strcat(tmp,p->keys[i]);
	strcat(tmp,'\0');
	fl.open("driver.txt",ios::app);
	fl.write(tmp,strlen(tmp));
	cout<<tmp<<"\n"<<strlen(tmp);

	fl.write("#",1);
	fl.close();
    }
  }
  for(i=0;i<p->cnt;i++)
    if(p->dptrs[i]!=0) driver(p->dptrs[i]);
}

void display(node * p)
{
  int i,j;
  person std;
  if(p->isLeaf())
  {
	 fstream file(datafile,ios::in);
	if(file.fail())
{
	gotoxy(24,10);
	cout<<"!!! ...The File Is Empty... !!!";
	getch();
	return;
}

  cout<<"COUNT : "<<p->cnt;
  cout<<"\n==========================================================";
    for(i=0;i<p->cnt;i++)
    {
	block * t=p->ptr[i];
	for(j=0;j<t->cnt;j++)
	{
	 file.seekg(t->disp[j],ios::beg);
	 if(std.Unpack(file))
	 {
	std.Display();
	cout<< "\n\t\t\t\t Press any key ...\n";
  cout<<"\n==========================================================";
	getch();
	 }
	 else break;
	}
    }
    file.clear();
    file.close();
  }
  for(i=0;i<p->cnt;i++)
    if(p->dptrs[i]!=0) display(p->dptrs[i]);
}
node::node()
{
  for(int i=0;i<4;i++)
  {
    strcpy(keys[i],'\0');
    dptrs[i]=0;
    ptr[i]=0;
  }
  uptr=0;
  cnt=0;
}
int node::isLeaf()
{
  int flag=1;
  for(int i=0;i<4;i++)
	 if((dptrs[i]) != 0)
    {
	 flag=0;
	 break;
    }
  return flag;
}
void node::split(node *nd,node *cr,node *up)
{
  int i;
  node *z=nd->uptr;
  cr=new node;
  for(i=2;i<4;i++)
  {
    strcpy(cr->keys[cr->cnt],nd->keys[i]);
    strcpy(nd->keys[i],'\0');
    cr->ptr[cr->cnt]=nd->ptr[i];
    nd->ptr[i]=0;
    cr->dptrs[cr->cnt]=nd->dptrs[i];
    nd->ptr[i]=0;
    cr->cnt++;
    nd->cnt--;
  }

  if(z!=0)
  {
    node *x2;
    if(z->cnt==4)
    {
	 dummy=0; dummy1=0;
	 z->split(z,up,x2);
	 if(dummy!=0) up=dummy;
	 if(dummy1!=0) z=dummy1;
	 for(i=2;i>=0;i--)
	 {
	if(strcmp((up->keys[i]),'\0')!=0)
	  if(strcmp((nd->keys[(nd->cnt)-1]),(up->keys[i]))<0)
	  {
	    strcpy(up->keys[i+1],up->keys[i]);
	    up->ptr[i+1]=up->ptr[i];
	    up->dptrs[i+1]=up->dptrs[i];
	  }
	  else
	  {
	    strcpy(up->keys[i+1],(nd->keys[(nd->cnt)-1]));
	    up->ptr[i+1]=(nd->ptr[(nd->cnt)-1]);
	    up->cnt++;
	    break;
	  }
	 }
	 if(i<0)
	 {
	strcpy(up->keys[0],(nd->keys[(nd->cnt)-1]));
	up->ptr[0]=(nd->ptr[(nd->cnt)-1]);
	up->cnt++;
	 }
	 for(i=3;i>0 && strcmp((cr->keys[(cr->cnt)-1]),up->keys[i])!=0;i--);
	 up->dptrs[i]=cr;
	 cr->uptr=up;
    }
    else
    {
	 for(i=2;i>=0;i--)
		 {
	if(strcmp((z->keys[i]),'\0')!=0)
	  if(strcmp((nd->keys[(nd->cnt)-1]),(z->keys[i]))<0)
	  {
	    strcpy(z->keys[i+1],z->keys[i]);
	    z->ptr[i+1]=z->ptr[i];
	    z->dptrs[i+1]=z->dptrs[i];
	  }
	  else
	  {
		    strcpy(z->keys[i+1],(nd->keys[(nd->cnt)-1]));
	    z->ptr[i+1]=(nd->ptr[(nd->cnt)-1]);
	    z->cnt++;
	    break;
	  }
	 }
	 if(i<0)
	 {
	strcpy(z->keys[0],(nd->keys[(nd->cnt)-1]));
	z->ptr[0]=(nd->ptr[(nd->cnt)-1]);
	z->cnt++;
	 }
	 for(i=3;i>0 && strcmp((cr->keys[(cr->cnt)-1]),z->keys[i])!=0;i--);
	 z->dptrs[i]=cr;
	 cr->uptr=nd->uptr;
    }
  }
  else
  {
    up=new node;
    strcpy(up->keys[0],nd->keys[(nd->cnt)-1]);
    up->ptr[0]=nd->ptr[(nd->cnt)-1];
    up->dptrs[0]=nd;
    strcpy(up->keys[1],cr->keys[(cr->cnt)-1]);
    up->ptr[1]=cr->ptr[(cr->cnt)-1];
    up->dptrs[1]=cr;
    up->cnt=2;
    nd->uptr=cr->uptr=up;
    bt.root=up;//reset root
  }
  dummy=cr;
  dummy1=nd;
}
btree::btree()
{
  root=new node;
}
node* btree::findLeaf(char* value,int &flg)
{
  node *x=root;
  int i;
  flg=0;
  while(!(x->isLeaf()))
  {
   for(i=3;i>=0;i--)
    {
     if((strcmp((x->keys[i]),'\0'))!=0)
      if(strcmp(value,(x->keys[i]))>0)
      {
       if(i<(x->cnt)-1)
	x=x->dptrs[i+1];
       else
	x=x->dptrs[(x->cnt)-1];
       break;
		}
      else if(strcmp(value,(x->keys[i]))==0) flg=1;
    }
    if(i<0) x=x->dptrs[0];
  }
  for(i=0;i<x->cnt;i++)
    if(strcmp(value,(x->keys[i]))==0)
    {
      flg=1;
      return x;
    }
  return x;
}
void btree::dis(node *p)
{
  int i;
  static int j=1;
  cout<< "\n  Level-" << j++ <<":";
  for(i=0;i<p->cnt;i++)
    cout << "    " << p->keys[i];
  cout <<"\n------------------------------------------------------------------------\n";
  for(i=0;i<p->cnt;i++)
    if(p->dptrs[i]!=0) dis(p->dptrs[i]);
  j--;
}

void btree::create()
{
 block *temp=head;
 bt.root = new node;
 while(temp!=0)
 {
  bt.insert(temp->keys[temp->cnt-1],temp);
  temp = temp->link;
 }
}
int btree::insert(char* val, block *d)
{
  int i,flg;
  node *x=findLeaf(val,flg),*a1,*a2;
  if(flg==1)
  {
       return 0;
  }
  if((x->cnt)==4)
  {
    x->split(x,a1,a2);
    x=findLeaf(val,flg);
  }
  for(i=2;i>=0;i--)
    if(strcmp((x->keys[i]),'\0')!=0)
      if(strcmp(val,(x->keys[i]))<0)
      {
	strcpy(x->keys[i+1],x->keys[i]);
	x->ptr[i+1]=x->ptr[i];

      }
      else
      {
	strcpy(x->keys[i+1],val);
	x->ptr[i+1]=d;
	x->cnt++;
	break;
      }
  if(i<0)
  {
    strcpy(x->keys[0],val);
    x->ptr[0]=d;
    x->cnt++;
  }
  else if(i==(x->cnt)-2)
    while((x->uptr)!=0)
    {
      x=x->uptr;
      strcpy(x->keys[(x->cnt)-1],val);
	 x->ptr[(x->cnt)-1]=d;
    }
    return 1;
}
block *btree::search(char *key,int &fnd)
{
  int i,flg=0;
  fnd=0;
  node *x=findLeaf(key,flg),*a1,*a2;
  for(i=0;i<x->cnt;i++)
   if(strcmp(key,x->keys[i])<=0)
   {
    fnd=1;
    return(x->ptr[i]);
   }
  return head;
}
void iobuf::Clear()
{
  strcpy(Buffer,"");
  BufSiz=0;
}
int varlen::Read(fstream &file)
{
  if(file.fail()) return 0;
  Clear();
  file.getline(Buffer,100,'#');
  BufSiz=strlen(Buffer);
  return 1;
}
int varlen::Write(fstream &file)
{
  if(file.write(Buffer,BufSiz))
  {
    file.write("#",1);
    return 1;
  }
  return 0;
}
int delmfld::Pack(person &std,fstream  &file)
{
  Clear();
  strcpy(Buffer,std.bid);
  strcat(Buffer,"|");
  strcat(Buffer,std.Name);
  strcat(Buffer,"|");
  strcat(Buffer,std.num_seats);
  strcat(Buffer,"|");
  strcat(Buffer,std.tot_cost);
  strcat(Buffer,"|");
  strcat(Buffer,std.theatre_no);
  strcat(Buffer,"|");
  strcat(Buffer,std.City);
  strcat(Buffer,"|");
  strcat(Buffer,std.Payment);
  strcat(Buffer,"|");
  strcat(Buffer,std.th_name);
  strcat(Buffer,'\0');
  BufSiz=strlen(Buffer);
  return(Write(file));
}
int delmfld::Unpack(person &std,fstream &file)
{
  if(!Read(file)) return 0;
  std.Clear();
  if(BufSiz>0)
  {
    int p=0,q;
    for(q=0;Buffer[q]!='|';q++) std.bid[p++]=Buffer[q];
    std.bid[p]='\0'; p=0; q++;
    for(;Buffer[q]!='|';q++) std.Name[p++]=Buffer[q];
    std.Name[p]='\0'; p=0; q++;
    for(;Buffer[q]!='|';q++) std.num_seats[p++]=Buffer[q];
    std.num_seats[p]='\0'; p=0; q++;
    for(;Buffer[q]!='|';q++) std.tot_cost[p++]=Buffer[q];
    std.tot_cost[p]='\0'; p=0; q++;
    for(;Buffer[q]!='|';q++) std.theatre_no[p++]=Buffer[q];
    std.theatre_no[p]='\0'; p=0; q++;
    for(;Buffer[q]!='|';q++) std.City[p++]=Buffer[q];
    std.City[p]='\0'; p=0; q++;
    for(;Buffer[q]!='|';q++) std.Payment[p++]=Buffer[q];
    std.Payment[p]='\0'; p=0; q++;
    for(;q<BufSiz;q++) std.th_name[p++]=Buffer[q];
    std.th_name[p]='\0';

    return 1;
  }
  return 0;
}
void person::Clear()
{
  strcpy(bid,"");
  strcpy(Name,"");
  strcpy(num_seats,"");
  strcpy(tot_cost,"");
  strcpy(theatre_no,"");
  strcpy(City,"");
  strcpy(Payment,"");
  strcpy(th_name,"");
}
void person::Input(int flUpd)
{
   Clear();
   if(!flUpd)
   {
   get_bid:
   cout<<"\t Enter 6 digit BID  : ";
   cin>>bid;
   if(checkbid(bid)==0){cout<<"\n Invalid Booking ID\n";goto get_bid;}
   }

   get_name:
   cout<<"\n\t Name     : "; gets(Name);
	 get_num_seats:
   cout<<"\n\t Number of seats  : "; cin>>num_seats;

   get_tot_cost:
   cout<<"\n\t Total cost: "; cin>>tot_cost;
  get_theatre_no:
   cout<<"\n\t Enter Screen number: "; cin>>theatre_no;
    get_City:
  cout<<"\n\t Enter City: "; cin>>City;
   get_Payment:
  cout<<"\n\t Payment Method: "; cin>>Payment;
   get_th_name:
   cout<<"\n\t Theatre Name:";gets(th_name);
 }
void person::Display()
{

  cout << "\n\tBid:\t " << bid
	   << "\n\tName:\t" << Name
	   << "\n\tNumber of seats:\t" << num_seats
	   << "\n\tTotal Cost:\t" << tot_cost
	   << "\n\tScreen Number:\t"<< theatre_no
	   <<"\n\tCity:\t"<<City
	  <<"\n\tPay Method:\t"<<Payment
	   <<"\n\tTheatre Name:\t"<<th_name ;
}
void person::Assign(person &std)
{
  strcpy(bid,std.bid);
  strcpy(Name,std.Name);
  strcpy(num_seats,std.num_seats);
  strcpy(tot_cost,std.tot_cost);
  strcpy(theatre_no,std.theatre_no);
  strcpy(City,std.City);
  strcpy(Payment,std.Payment);
  strcpy(th_name,std.th_name);

}
int person::Pack(fstream &file)
{
  delmfld buffer;
  return (buffer.Pack(*this,file));
}
int person::Unpack(fstream &file)
{
  delmfld buffer;
  return (buffer.Unpack(*this,file));
}
block :: block()
      {
       for(int i=0;i<4;i++)
	  {
	strcpy(keys[i],'\0');
	disp[i]=0;
       }
       cnt=0;
       link=0;
		}

void block :: split()
      {
       block *newnode;
       newnode=new block;
       int j=0;
       for(int i=2;i<this->cnt;i++)
       {
	strcpy(newnode->keys[j],this->keys[i]);
	strcpy(this->keys[i],'\0');
	newnode->disp[j]=this->disp[i];
	this->disp[i]=-1;
	newnode->cnt++;
	j++;
       }
       newnode->link = this->link;
       this->link=newnode;
       this->cnt-=2;
       bt.create();
		}
void block :: merge()		// function to merge two blocks
      {
       block *t1,*t2;
       t1=head;
       if(t1 == this)        // merge first block
       {
	if(t1->link->cnt < 4 )  // check wether next block can be merged
	{
	 for(int p=t1->link->cnt;p>=0;p--)
	 {
	  strcpy(t1->link->keys[p],t1->link->keys[p-1]);
	  t1->link->disp[p] = t1->link->disp[p-1];
	 }
	 strcpy(t1->link->keys[0],t1->keys[0]);
	 t1->link->disp[0]=t1->disp[0];
	 t1->link->cnt++;
	 head=t1->link;
	 t1->link=0;
	 delete t1;
	}
	else if(t1->link->cnt==4)     	// redistribution of keys
	{
	 strcpy(t1->keys[1],t1->link->keys[0]);
	 t1->disp[1]=t1->link->disp[0];
	 t1->cnt++;
	 for(int c=0;c<t1->link->cnt;c++)
	 {
	  strcpy(t1->link->keys[c],t1->link->keys[c+1]);
	  t1->link->disp[c]=t1->link->disp[c+1];
	 }
	 t1->link->cnt--;
	}
	  }
       else		// find which block to be merged
       {
	while(t1->link != this)
	 t1=t1->link;
	if(t1->cnt < 4)		// merge with left node
	{
	 strcpy(t1->keys[t1->cnt],this->keys[0]);
	 t1->disp[t1->cnt] = this->disp[0];
	 t1->link = this->link;
	 t1->cnt++;
	 this->link=0;
	 delete this;
	}
	else
	{
	 if(this->link !=0)
	  {                      // if not
	  t2=this->link;
	  if(t2->cnt < 4)      // merge with right node
	  {
	   for(int i=t2->cnt;i>=0;i--)
	   {
	    strcpy(t2->keys[i],t2->keys[i-1]);
	    t2->disp[i]=t2->disp[i-1];
	   }
		strcpy(t2->keys[0],this->keys[0]);
	   t2->disp[0]=this->disp[0];
	   t2->cnt++;
	   t1->link = this->link;
	   this->link=0;
	   delete this;
	  }
	  else
	  {	//Redistribution
	   t2=this->link;
	   strcpy(this->keys[this->cnt],t2->keys[0]);
	   this->disp[this->cnt]=t2->disp[0];
	   this->cnt++;
	   for(int i=0;i<t2->cnt;i++)
	   {
	    strcpy(t2->keys[i],t2->keys[i+1]);
	    t2->disp[i]=t2->disp[i+1];
	   }
	   t2->cnt--;
	  }
	 }
	 else			// if it is last block
	 {
	  strcpy(this->keys[1],this->keys[0]);
	  this->disp[1]=this->disp[0];
	  strcpy(this->keys[0],t1->keys[t1->cnt-1]);
	  this->disp[0]=t1->disp[t1->cnt-1];
	  this->cnt++;
	  t1->cnt--;
	 }
	}
		 }
		}
void SSET::create()	// function to create blocks from file
{
  int pos;
  char *buf;
  fstream file;
  file.open(datafile,ios::in);
  if(file.fail())
  {
  clrscr();
  gotoxy(24,10);
   printf("!!! ...File is empty... !!!");
   gotoxy(10,15);
   printf("!!! ...Press any key to start entering the records... !!!");
	getch();
	return;
}

  while(!file.fail())
  {
	 pos=file.tellg();
	 file.getline(buf,100,'|');
	 if(strcmp(buf,"")!=0)
	 {

      s.Insert(buf,pos);

      file.getline(buf,100,'#');
    }
  }
  file.close();
  bt.create();
}
block *SSET :: FindNode(char *val)	// function to find the block
      {
       BK t=head;
       int flag=0;

       node *temp=bt.root;
       int flg;
       temp = bt.findLeaf(val,flg);
       for(int i=0;i<temp->cnt;i++)
	if((strcmpi(temp->keys[i],val)>=0))
	{
	 t=temp->ptr[i];
	 flag=1;
	 break;

	}
       else if((temp->cnt!=0)&& (flag!=1) &&(!(strcmpi(temp->keys[i],val)==0)))
	 t=temp->ptr[temp->cnt-1];
       return t;

      }
int SSET :: Insert(char *val,int disp) // function to insert key in block
		{int i=0;
       BK x=FindNode(val);
	for(i=0;i<x->cnt;i++)
      if(strcmpi(x->keys[i],val)==0)
	  { cout<< " \nkey "<<val<<" is repeated \n"<<endl;
	   return 0;
	   }
       if(x->cnt < 4)
       {
	for(i=0;i<x->cnt;i++)
	 if(strcmpi(x->keys[i],val)>0) break;
	       if(strcmpi(x->keys[i],val)==0)
	  { cout<< "\n key "<<val<<" is repeated \n"<<endl;
	   return 0;
	   }
	else
	{
	 int d=i;
	 i=x->cnt-1;
	 while(i>=d)
	 {
	  strcpy(x->keys[i+1],x->keys[i]);
	  x->disp[i+1]=x->disp[i];
	  i--;
	 }
	 strcpy(x->keys[d],val);
	 x->disp[d]=disp;
	 x->cnt++;
	 if(d==x->cnt-1)
	 {
	  bt.create();
	 }
	}
       }
       else
       {
	x->split();
	x=FindNode(val);
	if(x->cnt < 4)
	{
	 for(i=0;i<x->cnt;i++)
	  if(strcmpi(x->keys[i],val)>=0) break;
	 if(strcmpi(x->keys[i],val)==0)
		  cout<<"";
	 else
	 {
	  int d=i;
	  i=x->cnt-1;
	  while(i>=d)
	  {
	   strcpy(x->keys[i+1],x->keys[i]);
	   x->disp[i+1]=x->disp[i];
	   i--;
	  }
	  strcpy(x->keys[d],val);
	  x->disp[d]=disp;
	  x->cnt++;
	 }
	}
	bt.create();
       }
       return 1;
	 }
void SSET :: del(char *val)	// function to delete key from block
      {
       int z,flg=0,i;
       BK x=FindNode(val);
       for(i=0;i<x->cnt;i++)
	if(strcmpi(x->keys[i],val)==0)
	{
	 flg=1;
	 z=i;
	 break;
	}
       if(flg==1)
       {
	if(x->cnt-1 < 2)
	{
	 for(int j=i;i<x->cnt;i++)
	 {
	  strcpy(x->keys[j],x->keys[i+1]);
	  strcpy(x->keys[i+1],"");
	  x->disp[j]=x->disp[i+1];
	  x->disp[i+1]=-1;
	  j++;
	 }
	 x->cnt--;
	 x->merge();
	 delete bt.root;
	 //bt.create();
	}
	else
	{
	 for(int j=i;i<x->cnt;i++)
	 {
	  strcpy(x->keys[j],x->keys[i+1]);
	  strcpy(x->keys[i+1],"");
	  x->disp[j]=x->disp[i+1];
	  x->disp[i+1]=-1;
	  j++;
	 }
	 x->cnt--;
	 if(z==x->cnt)
	 {
	 delete bt.root;
	  bt.root = new node;
	  //bt.create();
	 }
	}
       }
       else
	cout<<"\n\nKey "<<val<<" not found\n";
      }
void SSET :: display()
      {
		 int j=0;
       BK t;
       t=head;
       getch();
       cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
       cout<<"\n Block Structure \n";
       cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
	  while(t != 0)
       {
	cout<<"\n Node :"<<j;
	for(int i=0;i<t->cnt;i++)
	{
	 cout<<"\n keys["<<i<<"] : " <<t->keys[i];
		}
	t=t->link;
	j++;
       }
      }

SSET :: SSET()		// constructor for sequence set
      {
		 head = new block;
      }
void person :: dataDisp()
{
	cout<<setiosflags(ios::left);
	cout<<setw(10)<<"BID"<<setw(10)<<"Name"<<setw(10)<<"Seats"
	<<setw(8)<<"Cost"<<setw(12)<<"Screen"<<setw(13)<<"City"<<setw(10)<<"Payment"<<setw(12)<<"Theatre"<<endl;
	while(1)
	{
		unpack1();
		if(stdfile.eof())
			break;
			cout<<setw(10)<<bid<<setw(10)<<Name<<setw(10)<<num_seats<<setw(8)<<tot_cost<<setw(12)<<theatre_no<<setw(13)<<City<<setw(10)<<Payment<<setw(12)<<th_name<<endl;
		}
}
void person::unpack1()
{
char dummy[75];
	stdfile.getline(bid,10,'|');
	stdfile.getline(Name,10,'|');
	stdfile.getline(num_seats,10,'|');
	stdfile.getline(tot_cost,10,'|');
	stdfile.getline(theatre_no,10,'|');
	stdfile.getline(City,12,'|');
	stdfile.getline(Payment,10,'|');
	stdfile.getline(th_name,12,'#');
}
int main ()
{
    textbackground(BLACK);
    textcolor(BLUE);

   clrscr();
	 int pos,l,o;
	 char ch;
	person stud;
	 s.create();
	char key[100];
	clrscr();
	do
	{

	  clrscr();
	  in.initial();
	  gotoxy(18,2);
	  cout<<"<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>";gotoxy(18,4);
	  cout<<"*";
	  gotoxy(32,4);
	  cout<<"Movie Booking Manager";gotoxy(59,4);cout<<"*";gotoxy(18,6);
	  cout<<"<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>";gotoxy(18,8);
	  cout<<"* 1: Add new movie booking record";gotoxy(59,8);cout<<"*";gotoxy(18,10);
	  cout<<"* 2: Entry sequence display";gotoxy(59,10);cout<<"*";gotoxy(18,12);
	  cout<<"* 3: Tree structure display";gotoxy(59,12);cout<<"*";gotoxy(18,14);
	  cout<<"* 4: Delete record";gotoxy(59,14);cout<<"*";gotoxy(18,16);
	  cout<<"* 5: Search for record ";gotoxy(59,16);cout<<"*";gotoxy(18,18);
	  cout<<"* 6: Update record";gotoxy(59,18);cout<<"*";gotoxy(18,20);
	  cout<<"* 7: Generate data delimited file";gotoxy(59,20);cout<<"*";gotoxy(18,22);
	  cout<<"* 8: Quit program";gotoxy(59,22);cout<<"*";gotoxy(18,24);
	  cout<<"\n\t\t Enter choice [1-7] : ";
	 cin>>ch;
	 switch(ch)
	  {
	 case '1':
	   clrscr();
	   gotoxy(22,2);
	   cout<<"<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>";
	   gotoxy(22,4);
	   cout<<"<<  ADD RECORD INTO THE FILE  >>";
	   gotoxy(22,6);
	   cout<<"<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>\n\n";
	   append();
	   in.write();
		break;
	 case '5':
	 clrscr();
	   gotoxy(22,2);
	   cout<<"<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>";
	   gotoxy(22,4);
	   cout<<"<<  SEARCH FOR RECORD USING B-TREE  >>";
	   gotoxy(22,6);
	   cout<<"<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>\n\n";
	   cout<<"\n\t Enter the BID to search : ";
	   cin>>key;
		search(key);
	   break;
	 case '4':
	 clrscr();
	   gotoxy(28,2);
	   cout<<"<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>";
	   gotoxy(28,4);
	   cout<<"<<  DELETE RECORD  >>";
	   gotoxy(28,6);
	   cout<<"<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>\n\n";
	   cout<<"\n\t Enter the BID to delete : ";
	   cin>>key;
	   delrec(key);
	   s.del(key);
	   delete head;
	   o=indsearch(key);
	for(l=o;l<indsize;l++)
		id[l]=id[l+1];
		indsize--;
	   in.write();
	   head = new block;
	   bt.root = new node;
	   s.create();
	   break;
	 case '6':
	 clrscr();
	   gotoxy(28,2);
	   cout<<"<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>";
	   gotoxy(28,4);
	   cout<<"<<  UPDATE RECORD  >>";
		gotoxy(28,6);
	   cout<<"<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>\n\n";
	   cout<<"\n\t Enter the BID to update : ";
	   cin>>key;
	    update(key);
	   break;
	 case '3':
	   clrscr();
	   gotoxy(20,2);
	   cout<<"<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>";
	   gotoxy(20,3);
	   cout<<"<<   TREE STRUCTURE DISPLAY  >>";
	   gotoxy(20,4);
	   cout<<"<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>\n\n";
	   bt.dis(bt.root);
		s.display();
	   break;
		 case '8': cout << "\n\t\t !!!   Press any key to exit...   !!!";
			getch();
			exit(1);
			break;

		 case '2':
				opener(stdfile,datafile,ios::in);
				cout<<endl<<"Booking Details\n";
				stud.dataDisp();

				break;
	case '7':
		char filename[20],ip[100];
		fstream ipfile, opfile;
	clrscr();

		gotoxy(20,2);
		cout<<"<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>";
		gotoxy(13,4);
		cout<<"ENTER THE FILENAME TO GENERATE DELIMITER SEPARATED FILE: ";
		gotoxy(20,6);
		cout<<"<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>";
		gotoxy(27,8);
		cin>>filename;
		opener(ipfile,filename,ios::in);
		//opener(opfile,globalfilename,ios::out);
		opener(opfile,"datafile.txt",ios::out);
		while(!ipfile.eof()){
			ipfile.getline(ip,100,'\n');
			char *token = strtok(ip," ");
			while(token != NULL){
			       opfile <<token;
				token = strtok(NULL," ");
				if(token != NULL)
					opfile<<"|";
			}
			opfile<<"#";
		}
		gotoxy(15,10);
		cout<< "The delimiter separated file is successfully generated!!";
		gotoxy(25,12);
		cout<< "[FILE-NAME]: datafile.txt";
		break;

	 default:
		cout<<"\n\t\t !!! Invalid Choice. Press any key ... !!!";
		break;
		 }
		if(ch>1 && ch<7)
		cout << "\n\n\t Press any key to return to menu ... ";
		getch();
	 }
	 while(ch!='7');
  return 0;
}

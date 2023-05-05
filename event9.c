#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <process.h>
#include <dos.h>

struct event{
    unsigned id;
    char name[100];
    char date[11];
    char cat[20];
};

struct student{
    char name[100];
    char roll[20];
    char phone[14];
    char email[100];
    unsigned events[50];
};

void swap(unsigned *a, unsigned *b){
    unsigned temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void add_event(){

    FILE *fp, *ftevents;
    unsigned tevents=0; 
    struct event ev;
    fp = fopen("EVENT.DLL","ab");
    ftevents = fopen("TEVENTS.DLL", "rb");
    if(ftevents)
        fread(&tevents, sizeof(tevents), 1, ftevents);
    puts("\n\n");
    printf("\t\tEvent Name: ");
    fflush(stdin);
    gets(ev.name);
    fflush(stdin);
    printf("\t\tDate: ");
    fflush(stdin);
    gets(ev.date);
    fflush(stdin);
    printf("\t\tCategory: ");
    fflush(stdin);
    gets(ev.cat);
    fflush(stdin);
    tevents++;
    ev.id = tevents;
    fclose(ftevents);
    ftevents = fopen("TEVENTS.DLL", "wb");
    fwrite(&ev, sizeof(ev), 1, fp);
    fwrite(&tevents, sizeof(tevents), 1, ftevents);
    fclose(ftevents);
    fclose(fp);
}

void list_all_events(){

    struct event ev;
    FILE *fp, *ftevents;
    unsigned tevents = 0, i; 
    fp = fopen("EVENT.DLL", "rb");
    ftevents = fopen("TEVENTS.DLL", "rb");
    if(ftevents)
        fread(&tevents, sizeof(tevents), 1, ftevents);
    fclose(ftevents);
    puts("\n\n");
    for(i=0; i<tevents; i++){
        fread(&ev, sizeof(ev), 1, fp);
        printf("\t\tEvent ID: %d\n", ev.id);
        printf("\t\tEvent Name: %s\n", ev.name);
        printf("\t\tEvent Date: %s\n", ev.date);
        printf("\t\tEvent Category: %s\n", ev.cat);
        puts("\n");
    }
    fclose(fp);
    getch();
}

void add_student()
{
    FILE *fp; 
    struct student std;
    unsigned n=51, i;
    fp = fopen("STUDENT.DLL","ab");
    puts("\n\n");
    printf("\t\tStudent Name: ");
    fflush(stdin);
    gets(std.name);
    fflush(stdin);
    printf("\t\tStudent roll: ");
    fflush(stdin);
    gets(std.roll);
    fflush(stdin);
    printf("\t\tEmail ID: ");
    fflush(stdin);
    gets(std.email);
    fflush(stdin);
    printf("\t\tPhone No.: ");
    fflush(stdin);
    gets(std.phone);
    fflush(stdin);
    do{
	printf("\t\tEnter total events to assign to student (<50): ");
        scanf("%u", &n);
    }while(n>=50);
    puts("\t\tNow enter the event ids:");
    for(i=0; i<n; i++){
        scanf("%u", std.events + i);
    }
    for(;i<n;i++)
        std.events[i] = 0;
    fflush(stdin);
    fwrite(&std, sizeof(std), 1, fp);
    fclose(fp);
}

void list_all_students()
{
    FILE *fpe, *ftevents, *fps;
    struct student std;
    struct event *events;
    unsigned i,tevents = 0;
    fpe = fopen("EVENT.DLL","rb");
    ftevents = fopen("TEVENTS.DLL", "rb");
    fps = fopen("STUDENT.DLL","rb");
    if(ftevents)
        fread(&tevents, sizeof(tevents), 1, ftevents);
    fclose(ftevents);
    events = (struct event*)calloc(tevents,sizeof(struct event));
    fread(events, sizeof(struct event), tevents, fpe);
    fclose(fpe);
    puts("\n\n");
    while(fread(&std, sizeof(std), 1, fps)==1){
        printf("\t\tStudent Name: %s\n", std.name);
        printf("\t\tStudent roll: %s\n", std.roll);
        printf("\t\tStudent Email ID: %s\n", std.email);
        printf("\t\tStudent Phone No.: %s\n", std.phone);
        printf("\t\tEvents: \n");
        for(i=0; i<50 && std.events[i]!=0; i++){
            printf("\t\t%s\n",events[std.events[i]-1].name);
        }
        puts("\n");
    }
    fclose(fps);
    getch();
}

void del_event(){
    struct event *events;
    struct student std;
    unsigned tevents = 0, id, i, j;
    FILE *fpe,*ftevents,*fps,*ftemp;
    fpe = fopen("EVENT.DLL","rb");
    ftevents = fopen("TEVENTS.DLL", "rb");
    fps = fopen("STUDENT.DLL","rb");
    ftemp = fopen("temp.dll","wb");
    if(ftevents)
        fread(&tevents, sizeof(tevents), 1, ftevents);
    events = (struct event*)calloc(tevents,sizeof(struct event));
    fread(events, sizeof(struct event), tevents, fpe);
    fclose(fpe);
    puts("\n\n");
    printf("Enter ID of event to delete: ");
    scanf("%u", &id);
    if(tevents < id){
        fclose(ftemp);
        fclose(fps);
        fclose(ftevents);
        return;
    }
    freopen("EVENT.DLL", "wb", fpe);
    for(i=1; i<id; i++){
        fwrite(events+i-1, sizeof(struct event), 1, fpe);
    }
    for(i=id+1; i<tevents; i++){
        events[i].id--;
        fwrite(events+i-1, sizeof(struct event), 1, fpe);
    }
    while(fread(&std, sizeof(std), 1, fps)==1){
        for(i=0; i<50 && std.events[i]!=0; i++){
            if(std.events[i]==id){
                std.events[i]=0;
                for(j=i+1; j<50 && std.events[j]!=0; j++){
                    swap(std.events + j, std.events + j -1 );
                }
            }
            else if(std.events[i]>id){
                std.events[i]--;
            }
        }
        fwrite(&std, sizeof(std), 1, ftemp);
    }
    freopen("temp.dll","rb", ftemp);
    freopen("STUDENT.DLL", "wb", fps);
    while(fread(&std, sizeof(std), 1, ftemp)==1){
        fwrite(&std, sizeof(std), 1, fps);
    }
    freopen("TEVENTS.DLL", "wb", ftevents);
    tevents--;
    fwrite(&tevents, sizeof(tevents), 1, ftevents);
    fclose(ftevents);
    fclose(fps);
    fclose(ftemp);   
}

void total_events(){

    FILE *ftevents;
    unsigned tevents=0;
    ftevents = fopen("TEVENTS.DLL", "rb");
    if(ftevents)
        fread(&tevents, sizeof(tevents), 1, ftevents);
    puts("\n\n");
    printf("\t\t%u\n", tevents);   
    getch();

}

void modify_event(){
    struct event *events;
    unsigned tevents = 0, id;
    FILE *fpe,*ftevents;
    fpe = fopen("EVENT.DLL","rb");
    ftevents = fopen("TEVENTS.DLL", "rb");
    if(!ftevents)
        return;
    fread(&tevents, sizeof(tevents), 1, ftevents);
    events = (struct event*)calloc(tevents,sizeof(struct event));
    fread(events, sizeof(struct event), tevents, fpe);
    fclose(fpe);
    fclose(ftevents);
    puts("\n\n");
    printf("\t\tEnter id of Event to modify: ");
    fflush(stdin);
    scanf("%u", &id);
    fflush(stdin);
    if(id > tevents)
        return;
    puts("\n");
    printf("\t\tCurrent details of this Event\n");
    printf("\t\tEvent Name: %s\n", events[id-1].name);
    printf("\t\tEvent Date: %s\n", events[id-1].date);
    printf("\t\tEvent Category: %s\n", events[id-1].cat);
    puts("\n");
    printf("\t\tEnter new details\n\n");
    printf("\t\tEvent Name: ");
    fflush(stdin);
    gets(events[id-1].name);
    fflush(stdin);
    printf("\t\tDate: ");
    fflush(stdin);
    gets(events[id-1].date);
    fflush(stdin);
    printf("\t\tCategory: ");
    fflush(stdin);
    gets(events[id-1].cat);
    fflush(stdin);
    freopen("EVENT.DLL", "wb", fpe);
    fwrite(events, sizeof(struct event), tevents, fpe);
    fclose(fpe);

}
void modify_student(){
    struct student *std;
    unsigned n=51, i;
    FILE *fpe;
    fpe = fopen("student.dll","rb");
     if(!fpe)
        return;
    if(51 > i){-
        return;
    puts("\n");
    printf("\t\tCurrent details of student\n");
    printf("\t\tstudent Name: %s\n", std.name);
    printf("\t\tStudent roll: %s\n", std.roll);
    printf("\t\tEmail id: %s\n", std.email);
    printf("\t\tPhone No.: %s\n", std.phone);
    puts("\n");
    printf("\t\tEnter new details\n\n");
    printf("\t\tstudent Name: ");
    fflush(stdin);
    gets(std.name);
    fflush(stdin);
    printf("\t\tstudent roll: ");
    fflush(stdin);
    gets(std.roll);
    fflush(stdin);
    printf("\t\tEmail id: ");
    fflush(stdin);
    gets(std.email);
    fflush(stdin);
    printf("\t\tPhone No.: ");
    fflush(stdin);
    gets(std.phone);
    freopen("STUDENT.DLL", "wb", fpe);
    fwrite(std, sizeof(struct std), 1, fpe);
    fclose(fpe)
    
    }

}

void main(){

char choice;

while(1){
    system("cls");

    printf("\n\t * Welcome to Event Management System *");
    printf("\n\n\n\t\t\tMAIN MENU\n\t\t=====================\n\t\
    [1] Add a new Event\n\t\
    [2] List all Events\n\t\
    [3] Add a new Student\n\t\
    [4] List all Students\n\t\
    [5] Search for Events\n\t\
    [6] Modify an Event\n\t\
    [7] Delete an Event\n\t\
    [8] Modify a Student\n\t\
    [9] Total Events\n\t\
    [0] Exit\n\t\t=================\n\t\t");

    printf("Enter the choice:");
    fflush(stdin);
    scanf("%c",&choice);

    switch(choice){
	default:
		printf("\n\n\t\tPlease Enter a Valid option....");
		sleep(4);
		continue;
	case '1':
        add_event();
        continue;
	case '2':
	    list_all_events();
        continue;
	case '3':
        add_student();
        continue;
	case '4':
        list_all_students();
        continue;
	case '5':
        continue;
    case '6':
        modify_event();
        continue;
    case '7':
        del_event();
        continue;
    case '8':
        continue;
    case '9':
        total_events();
        continue;
	case '0':
	    puts("\n\n\t\tExiting.........");
	    sleep(2);
	return;
    }
}
}
#include <stdio.h>
#include <stdlib.h>

struct takimlar
{
    int takim_kodu;
    char takim_adi[21];
    int galibiyet_say;
    int beraber_say;
    int maglubiyet_say;
    int attigi_gol;
    int yedigi_gol;
    struct skorlar *skor; // takim dugumu skorlar dugumleri bu isaretci ile tutmaktadir
};

struct futbolcular
{
    int takim_kodu;
    int forma_no;
    char ad_soyad[21];
    int attigi_gol_say;
    struct futbolcular *sonraki;
};

struct skorlar
{
    int hafta_no;
    int ev_takimin_kodu;
    int misafir_takimin_kodu;
    int ev_takimin_gol_say;
    int misafir_takimin_gol;
    struct skorlar *ev_sonraki; // her dugum ev sonraki ve skor sonraki dugumleri takip etmektedir
    struct skorlar *misafir_sonraki;
};

int sayi_al(int alt,int ust);
struct futbolcular *futbolcu_ara(struct futbolcular *ilk, int aranan);
void sirali_ekle(struct futbolcular *liste_basi_ptr, struct futbolcular *yeni);
struct futbolcular *hashtan_cikar(struct futbolcular *liste_basi_ptr, int cikarilacak);
void sirali_ekle_hafta_noya_gore(struct skorlar *liste_basi_ptr, struct skorlar *yeni,int durum);

int main()
{
    int secim,i;
    int kod;
    int kod2;
    int durum;
    int forma_no;
    int forma_no2;
    char takim_adi[21];
    struct takimlar *takim_isaretci_dizisi[100]= {NULL};
    struct futbolcular hash_tablosu[100];
    struct futbolcular *gecici;
    struct takimlar *bir_takim;
    struct futbolcular *bir_futbolcu;
    struct futbolcular *en_cok_gol_atanlar[100]= {NULL};
    struct skorlar *bir_skor;
    struct skorlar *gecici1;
    struct skorlar *gecici2;
    struct skorlar *deplasmanda_en_cok[100]= {NULL};
    struct futbolcular *bir_futbolcu2;
    int gol_say;
    int hafta;
    char cevap;
    int oyun_say;
    int averaj;
    int puan;
    int bulundu=0;
    int max=0;
    int min;

    for(i=0; i<100; i++)
        hash_tablosu[i].sonraki=&hash_tablosu[i];

    do
    {
        printf("\n 1) Yeni bir takimin eklenmesi");
        printf("\n 2) Bir takima lig disindan yeni bir futbolcunun transfer edilmesi");
        printf("\n 3) Bir takimdaki bir fulbolcunun ligdeki baska bir takima transfer edilmesi");
        printf("\n 4) Bir takimdaki futbolcunun lig disindaki bir takima transfer edilmesi");
        printf("\n 5) Mac skorlarinin kaydedilmesi");
        printf("\n 6) Bir takimin su anki durumunun ve futbolcularinin listelenmesi");
        printf("\n 7) Bir takimin su anki durumunun ve oynadigi maclarin listelenmesi.");
        printf("\n 8) Tum takimlarin durumlarinin listelenmesi");
        printf("\n 9) En cok gol atan ve en az gol yiyen takimlarin listelenmesi");
        printf("\n 10) Deplasmanda en cok gol atan takimin listelenmesi");
        printf("\n 11) Her takimda en cok gol atan futbolcunun ve ligin gol kralinin listelenmesi");
        printf("\n 12) Cikis");
        secim=sayi_al(1,12);

        switch(secim)
        {
        case 1:

            printf("\n Yeni bir takim icin takim kodunu giriniz:");
            kod=sayi_al(1,99);
            if(takim_isaretci_dizisi[kod]==NULL) // eger oyle bir takim kodu yok ise yeni bir dugum olusturuluyor
            {
                printf("\n Bu takimin adi giriniz:");
                scanf("%s",takim_adi);
                durum=0;
                for(i=1; i<100; i++)
                {
// takimin adi araniyor eger varsa hata mesaji yazdirilacak
                    if( takim_isaretci_dizisi[i]!=NULL && strcmp(takim_adi,takim_isaretci_dizisi[i]->takim_adi)==0)
                    {
                        durum=1;
                        break;
                    }
                }
                if(durum==1)
                    printf("\n boyle bir takim adi var.");
                else
                {
                    bir_takim=malloc(sizeof (struct takimlar)); // takim adi dugumu olusturuluyor
                    bir_takim->takim_kodu=kod;
                    strcpy( bir_takim->takim_adi,takim_adi); // kullanicidan girilen ad dugume kopyalaniyor
                    bir_takim->attigi_gol=0; // tum degerler basta 0
                    bir_takim->beraber_say=0;
                    bir_takim->galibiyet_say=0;
                    bir_takim->maglubiyet_say=0;
                    bir_takim->yedigi_gol=0;

                    bir_takim->skor=NULL;

                    takim_isaretci_dizisi[kod]=bir_takim; // dairesel olmadigi icin NULL ataniyor

                }
            }
            else
                printf("\n Boyle bir takim var.");
            break;

        case 2:
            printf("\n Yeni bir futbolcunun takim kodu giriniz:");
            kod=sayi_al(1,99);
            if(takim_isaretci_dizisi[kod]!=NULL) // eger oyle bir takim varsa...
            {
                printf("\n Bu futbolcunun forma no giriniz:");
                forma_no=sayi_al(1,99);
                if(futbolcu_ara(&hash_tablosu[kod],forma_no)==NULL) // kullanicidan girilen forma no daha once girildi mi diye bakiliyor
                {
                    bir_futbolcu=malloc(sizeof(struct futbolcular)); // futbolcu dugumu olusturuluyor
                    bir_futbolcu->takim_kodu=kod;
                    bir_futbolcu->forma_no=forma_no;
                    printf("\n Bu futbolcunun ad soyad giriniz:");
                    scanf("%s",bir_futbolcu->ad_soyad);
                    bir_futbolcu->attigi_gol_say=0; // basta mac skor girilmedigi icin attigi gol sayisi 0 olur
                    sirali_ekle(&hash_tablosu[kod],bir_futbolcu); // bu dugum forma no'ya gore sirali ekleniyor

                }
                else
                    printf("\n Boyle bir futbolcu var.\n");
            }
            else
                printf("\n Boyle bir takim yok.\n");
            break;

        case 3:
            printf("\n Futbolcunun oynadigi takim kodu giriniz:");
            kod=sayi_al(1,99);
            if(takim_isaretci_dizisi[kod]==NULL)
                printf("\n Boyle bir takim yok.\n");
            else // oyle bir takim varsa...
            {
                printf("\n Bu futbolcunun forma no giriniz:");
                forma_no=sayi_al(1,99);
                if(futbolcu_ara(&hash_tablosu[kod],forma_no)==NULL) // oyle bir futbolcu var mi diye bakiliyor
                    printf("\n Boyle bir futbolcu yok.\n");
                else
                {
                    printf(" \n Bu futbolcuyu hangi takima transfer etmek istersiniz:");
                    kod2=sayi_al(1,99);
                    if(takim_isaretci_dizisi[kod2]==NULL)
                        printf("\n Boyle bir takim yok.");
                    else // oyle bir takim varsa...
                    {
                        printf("\n Bu futbolcunun yeni forma numarasi nasil olacak:");
                        forma_no2=sayi_al(1,99); // bu futbolcunun yeni forma no
                        if(futbolcu_ara(&hash_tablosu[kod2],forma_no2)==NULL) // oyle bir forma no yok ise futbolcu buraya transfer edilebilir
                        {
                            bir_futbolcu=hashtan_cikar(&hash_tablosu[kod],forma_no); // eski takimdan futbolcunun silinmesi
                            free(bir_futbolcu);
                            bir_futbolcu2=malloc(sizeof(struct futbolcular)); // futbolcunun yeni dugumu olusturuluyor
                            bir_futbolcu2->forma_no=forma_no2;
                            bir_futbolcu2->takim_kodu=kod2;

                            sirali_ekle(&hash_tablosu[kod2],bir_futbolcu2); // bu futbolcu hash_tabloya sirali ekleniyor

                        }
                        else
                            printf("\n Boyle bir futbolcu var.\n");
                    }
                }
            }
            break;

        case 4:
            printf("\n Futbolcunun takim kodu giriniz:");
            kod=sayi_al(1,99);
            if(takim_isaretci_dizisi[kod]==NULL)
                printf("\n Boyle bir takim yok.\n");
            else // eger oyle bir takim varsa...
            {
                printf("\n Bu futbolcunun forma no nedir:");
                forma_no=sayi_al(1,99);
                if(futbolcu_ara(&hash_tablosu[kod],forma_no)==NULL)
                    printf("\n Boyle bir futbolcu yok.\n");
                else // eger oyle bir futbolcu varsa...
                {
                    bir_futbolcu=hashtan_cikar(&hash_tablosu[kod],forma_no); // futbolcu silinmektedir
                    free(bir_futbolcu);
                    printf("\n Futbolcu basari ile lig disina transfer edildi.\n");
                }
            }
            break;

        case 5:
            do
            {
                printf("\n Macin hafta numarasi giriniz:");
                scanf("%d",&hafta);

                printf("\n Ev sahibi takimin kod numarasi giriniz:");
                kod=sayi_al(1,99);
                while(takim_isaretci_dizisi[kod]==NULL) // dogru takim kodu girilene kadar degeri sorulmaya devam edilecektir
                {
                    printf("\n Boyle bir takim yok.\n");
                    printf("\n Ev sahibi takimin kod numarasi giriniz:");
                    kod=sayi_al(1,99);
                }

                bir_skor=malloc(sizeof(struct skorlar)); // skor dugumu olusturuluyor

                bir_skor->hafta_no=hafta;

                bir_skor->ev_takimin_kodu=kod;

                printf("\n Ev sahibi takimin attigi gol sayisi kac:");
                scanf("%d",&bir_skor->ev_takimin_gol_say);

                if(bir_skor->ev_takimin_gol_say!=0) // eger ev takimin attigý gol sayisi 0 degilse...
                {
                    do
                    {
                        printf("\n Ev sahibi takimin goller atan futbolcunun forma numarasi nedir:");
                        forma_no=sayi_al(1,99);
                        while(futbolcu_ara(&hash_tablosu[kod],forma_no)==NULL) // dogru futbolcu numarasi girilene kadar degeri sorulmaya devam edilecektir
                        {
                            printf("\n Boyle bir futbolcu yok.");
                            printf("\n Ev sahibi takimin goller atan futbolcunun forma numarasi nedir:");
                            forma_no=sayi_al(1,99);
                        }
                        printf("\n Bu futbolcu kac gol atti:");
                        gol_say=sayi_al(1,bir_skor->ev_takimin_gol_say); // futbolcunun attigi gol sayisi bu takimin attigi gol sayisindan daha fazla olamaz
                        bir_futbolcu=futbolcu_ara(&hash_tablosu[kod],forma_no); // futbolcunun attigi gol sayisini kaydetmek icin futbolcu dugumu araniyor
                        bir_futbolcu->attigi_gol_say+=gol_say;

                        do
                        {
                            printf("\n Ev sahibi takiminin gol atan baska futbolcu var mi? (E,H,e,h):"); // dogru harf girilene kadar...
                            fflush(stdin);
                            scanf("%c",&cevap);
                        }
                        while(cevap!='E' && cevap!='e' && cevap!='H' && cevap!='h');
                    }
                    while(cevap!='H' && cevap!='h');
                }

                printf("\n Misafir takimin kod numarasi giriniz:"); // ayni islemeler misafir takimi icin de yapilmaktadir...
                kod2=sayi_al(1,99);
                while(takim_isaretci_dizisi[kod2]==NULL)
                {
                    printf("\n Boyle bir takim yok.");
                    printf("\n Misafir takimin kod numarasi giriniz:");
                    kod2=sayi_al(1,99);
                }

                bir_skor->misafir_takimin_kodu=kod2;

                printf("\n Misafir takimin attigi gol sayisi kac:");
                scanf("%d",&bir_skor->misafir_takimin_gol);

                if(bir_skor->misafir_takimin_gol!=0)
                {
                    do
                    {
                        printf("\n Misafir takimin goller atan futbolcunun forma numarasi nedir:");
                        forma_no=sayi_al(1,99);
                        while(futbolcu_ara(&hash_tablosu[kod2],forma_no)==NULL)
                        {
                            printf("\n Boyle bir futbolcu yok.");
                            printf("\n Misafir takimin goller atan futbolcunun forma numarasi nedir:");
                            forma_no=sayi_al(1,99);
                        }
                        printf("\n bu Futbolcu kac gol atti:");
                        gol_say=sayi_al(1,bir_skor->misafir_takimin_gol);
                        bir_futbolcu=futbolcu_ara(&hash_tablosu[kod2],forma_no);
                        bir_futbolcu->attigi_gol_say+=gol_say;
                        do
                        {
                            printf("\n Misafir takiminin gol atan baska futbolcu var mi? (E,H,e,h):");
                            fflush(stdin);
                            scanf("%c",&cevap);
                        }
                        while(cevap!='E' && cevap!='e' && cevap!='H' && cevap!='h');
                    }
                    while(cevap!='H' && cevap!='h');
                }

                if(bir_skor->ev_takimin_gol_say==bir_skor->misafir_takimin_gol) // eger ev sahibi takimin ve misafir takimin attigi gol sayisi birbirine eitse...
                {
                    takim_isaretci_dizisi[kod]->beraber_say++;
                    takim_isaretci_dizisi[kod2]->beraber_say++;
                }
                else if(bir_skor->ev_takimin_gol_say>bir_skor->misafir_takimin_gol) // eger ev sahibi takimin attigi gol sayisi misafir takimindan fazla ise...
                {
                    takim_isaretci_dizisi[kod]->galibiyet_say++;
                    takim_isaretci_dizisi[kod2]->maglubiyet_say++;
                }
                else //eger misafir takimin attigi gol sayisi ev sahibi takimindan takimindan fazla ise...
                {
                    takim_isaretci_dizisi[kod]->maglubiyet_say++;
                    takim_isaretci_dizisi[kod2]->galibiyet_say++;
                }
                takim_isaretci_dizisi[kod]->attigi_gol+=bir_skor->ev_takimin_gol_say; // ev sahibi takim dugumunun gol sayisi guncelleniyor
                takim_isaretci_dizisi[kod]->yedigi_gol+=bir_skor->misafir_takimin_gol;
                takim_isaretci_dizisi[kod2]->attigi_gol+=bir_skor->misafir_takimin_gol; // ev sahibi takim dugumunun gol sayisi guncelleniyor
                takim_isaretci_dizisi[kod2]->yedigi_gol+=bir_skor->ev_takimin_gol_say;
                bir_skor->ev_sonraki=NULL;
                bir_skor->misafir_sonraki=NULL;

                if(takim_isaretci_dizisi[kod]->skor==NULL)
                    takim_isaretci_dizisi[kod]->skor=bir_skor;

                else  sirali_ekle_hafta_noya_gore(takim_isaretci_dizisi[kod]->skor,bir_skor,0);

                if(takim_isaretci_dizisi[kod2]->skor==NULL)
                    takim_isaretci_dizisi[kod2]->skor=bir_skor;
                else
                    sirali_ekle_hafta_noya_gore(takim_isaretci_dizisi[kod2]->skor,bir_skor,1);



                do // yeni bir skor girilecek mi diye soruluyor
                {
                    printf("\n Baska mac skoru girecek misiniz? (E,H,e,h):");
                    fflush(stdin);
                    scanf("%c",&cevap);
                }
                while(cevap!='E' && cevap!='e' && cevap!='H' && cevap!='h');

            }
            while(cevap=='e' || cevap=='E');

            break;
        case 6:
            printf("\n Takimin adini giriniz:");
            scanf("%s",takim_adi);
            bulundu=0; // takimin var olup olmadigini kontrol eden degisken
            gecici=malloc(sizeof(struct futbolcular));
            for(i=1; i<100; i++)
            {

                if(takim_isaretci_dizisi[i]!=NULL)
                {
                    if(strcmp(takim_isaretci_dizisi[i]->takim_adi,takim_adi)==0) // kullanicidan girilen takim adi dogru ise...
                    {
                        bulundu=1;
                        kod=i;
                        averaj=takim_isaretci_dizisi[kod]->attigi_gol-takim_isaretci_dizisi[kod]->yedigi_gol;
                        puan=takim_isaretci_dizisi[kod]->galibiyet_say*3+takim_isaretci_dizisi[kod]->beraber_say;
// oyun sayisi hesaplaniyor
                        oyun_say=takim_isaretci_dizisi[kod]->beraber_say+takim_isaretci_dizisi[kod]->galibiyet_say+takim_isaretci_dizisi[kod]->maglubiyet_say;
                        printf("\n TAKIM KODU TAKIM ADI: OO GG BB MM AA YY AVR PUAN: \n");
                        printf("-----------------------------------------------------------------------\n");
                        printf(" %d %s %-5d %-5d %-5d %-5d %-5d %-5d %-5d %-5d \n",kod,takim_adi,oyun_say,takim_isaretci_dizisi[kod]->galibiyet_say,takim_isaretci_dizisi[kod]->beraber_say,takim_isaretci_dizisi[kod]->maglubiyet_say,takim_isaretci_dizisi[kod]->attigi_gol,takim_isaretci_dizisi[kod]->yedigi_gol,averaj,puan);
                        printf("\n FUTBOLCU BILGILERI:");
                        if(hash_tablosu[kod].sonraki!=&hash_tablosu[kod]) // eger hashing dizisinin sonraki dugumu kendine gostermiyorsa demekki futbolcular var...
                        {
                            gecici=hash_tablosu[kod].sonraki;
                            while(gecici!=&hash_tablosu[kod]) // tum futbolcular bilgileri listeienmektedir
                            {
                                printf("\n FORMA NO   AD SOYAD   AA:\n");
                                printf(" %d    %-15s %-12d \n",gecici->forma_no,gecici->ad_soyad,gecici->attigi_gol_say);
                                gecici=gecici->sonraki;
                            }
                        }
                        else printf("\n Futbolcular yok.\n");

                        break;
                    }
                }
            }
            if(bulundu==0)
                printf("\n Oyle bir takim adi yok.\n");
            break;
        case 7:
            printf("\n Takim kodu giriniz:");
            kod=sayi_al(1,99);
            if(takim_isaretci_dizisi[kod]!=NULL ) // girilen takim kodu varsa...
            {
                // averaj,puan, ve oynadigi oyun sayisi hesaplaniyor...
                averaj=takim_isaretci_dizisi[kod]->attigi_gol-takim_isaretci_dizisi[kod]->yedigi_gol;
                puan=takim_isaretci_dizisi[kod]->galibiyet_say*3+takim_isaretci_dizisi[kod]->beraber_say;
                oyun_say=takim_isaretci_dizisi[kod]->beraber_say+takim_isaretci_dizisi[kod]->galibiyet_say+takim_isaretci_dizisi[kod]->maglubiyet_say;
                printf("\n TAKIM KODU TAKIM ADI: OO GG BB MM AA YY AVR PUAN: \n");
                printf("-----------------------------------------------------------------------\n");
                printf(" %d %s %-5d %-5d %-5d %-5d %-5d %-5d %-5d %-5d \n",kod,takim_isaretci_dizisi[kod]->takim_adi,oyun_say,takim_isaretci_dizisi[kod]->galibiyet_say,takim_isaretci_dizisi[kod]->beraber_say,takim_isaretci_dizisi[kod]->maglubiyet_say,takim_isaretci_dizisi[kod]->attigi_gol,takim_isaretci_dizisi[kod]->yedigi_gol,averaj,puan);

                if(takim_isaretci_dizisi[kod]->skor!=NULL)
                {
                    printf("\n OYNADIGI MACLARIN SKORLARI:\n");
                    printf("\n EV SAHIBI MISAFIR \n");
                    printf("\n Hafta no     Takim adi     AA     AA      Takim adi: \n");
                    printf("-------------------------------------------------------\n");
                    gecici1=takim_isaretci_dizisi[kod]->skor;
                    kod2=gecici1->misafir_takimin_kodu;
                    printf("\n %d        %s       %d       %d      %s \n",gecici1->hafta_no,takim_isaretci_dizisi[kod]->takim_adi,gecici1->ev_takimin_gol_say,gecici1->misafir_takimin_gol,takim_isaretci_dizisi[kod2]->takim_adi);
                    gecici2=gecici1->misafir_sonraki;
                    gecici1=gecici1->ev_sonraki;


                    while(gecici1!=NULL || gecici2!=NULL)
                    {
                        durum=0;
                        // kullanicidan girildigi takim kodu ev_sonraki  veya misafir_sonrakide bulunan takim kodu ile karsilastiriliyor hangisine esit ise o if kosulu saglanmis olur

                        if(gecici1!=NULL && gecici1->ev_takimin_kodu==kod)
                        {
                            kod2=gecici1->misafir_takimin_kodu;
                            printf("\n %d %s %d %d %s \n",gecici1->hafta_no,takim_isaretci_dizisi[kod]->takim_adi,gecici1->ev_takimin_gol_say,gecici1->misafir_takimin_gol,takim_isaretci_dizisi[kod2]->takim_adi);
                            gecici2=gecici1->misafir_sonraki;
                            gecici1=gecici1->ev_sonraki;
                            continue;
                            durum=1;
                        }

                        if(gecici2!=NULL && gecici2->misafir_takimin_kodu==kod)
                        {
                            kod2=gecici2->ev_takimin_kodu;
                            printf("\n %d      %s       %d         %d       %s \n",gecici2->hafta_no,takim_isaretci_dizisi[kod2]->takim_adi,gecici2->ev_takimin_gol_say,gecici2->misafir_takimin_gol,takim_isaretci_dizisi[kod]->takim_adi);
                            gecici1=gecici2->ev_sonraki;
                            gecici2=gecici2->misafir_sonraki;
                            durum=2;
                        }
                        if(durum==0)
                            break;

                    }
                }

                else printf("\n hic skor yok");
            }
            else
                printf("\n Boyle bir takim kodu yok.\n");

            break;


        case 8:
            printf("\n TAKIM KODU TAKIM ADI:        OO   GG   BB   MM   AA   YY  AVR  PUAN: \n");
            printf("-----------------------------------------------------------------------\n");
            for(i=1; i<100; i++) // tum takimlar isaretci dizisinde bakiliyorlar
                if(takim_isaretci_dizisi[i]!=NULL) // eger bulunduysa...
                {
                    bir_takim=takim_isaretci_dizisi[i];
                    averaj=bir_takim->attigi_gol-bir_takim->yedigi_gol;
                    puan=bir_takim->galibiyet_say+bir_takim->beraber_say;

                    oyun_say=bir_takim->beraber_say+bir_takim->galibiyet_say+bir_takim->maglubiyet_say;
                    printf(" %d      %s       %-5d %-5d %-5d %-5d %-5d %-5d %-5d %-5d \n",bir_takim->takim_kodu,bir_takim->takim_adi,oyun_say,bir_takim->galibiyet_say,bir_takim->beraber_say, bir_takim->maglubiyet_say,bir_takim->attigi_gol,bir_takim->yedigi_gol,averaj,puan);
                }
            break;
        case 9:
            durum=0;
            printf("\n En cok gol atan takim:");
            printf("\n TAKIM KODU TAKIM ADI:        OO   GG   BB   MM   AA   YY  AVR  PUAN: \n");
            printf("-----------------------------------------------------------------------\n");
            for(i=1; i<100; i++)
            {
                if(takim_isaretci_dizisi[i]!=NULL)
                {
                    durum=1;
                    max=i; // ilk takim kodu max olarak aliniyor
                    break;
                }
            }
            for(i=1; i<100; i++)
            {
                // max takim kodu ve sonraki bulunan takimin attigi gol sayisi karsilastiriliyor
                if(takim_isaretci_dizisi[i]!=NULL && takim_isaretci_dizisi[i]->attigi_gol>takim_isaretci_dizisi[max]->attigi_gol)
                    max=i;
            }
            if(durum==1)
            {
                averaj=takim_isaretci_dizisi[max]->attigi_gol-takim_isaretci_dizisi[max]->yedigi_gol;
                puan=takim_isaretci_dizisi[max]->galibiyet_say*3+takim_isaretci_dizisi[max]->beraber_say;
                oyun_say=takim_isaretci_dizisi[max]->beraber_say+takim_isaretci_dizisi[max]->galibiyet_say+takim_isaretci_dizisi[max]->maglubiyet_say;
                // for dongusunden sonra max indeksin altinda en cok gol atan takim bulunacaktir
                printf(" %d      %s       %-5d %-5d %-5d %-5d %-5d %-5d %-5d %-5d \n",max,takim_isaretci_dizisi[max]->takim_adi,oyun_say,takim_isaretci_dizisi[max]->galibiyet_say,takim_isaretci_dizisi[max]->beraber_say,takim_isaretci_dizisi[max]->maglubiyet_say,takim_isaretci_dizisi[max]->attigi_gol,takim_isaretci_dizisi[max]->yedigi_gol,averaj,puan);
                printf("\n En az gol yiyen takim:");

                for(i=1; i<100; i++)
                {
                    if(takim_isaretci_dizisi[i]!=NULL)
                    {
                        min=i; // ilk bulunan takim minimum olarak aliniyor
                        break;
                    }
                }

                for(i=1; i<100; i++)
                {
                    // min takim kodu ve sonraki bulunan takimin yedigi gol sayisi karsilastiriliyor
                    if(takim_isaretci_dizisi[i]!=NULL && takim_isaretci_dizisi[i]->yedigi_gol<takim_isaretci_dizisi[min]->yedigi_gol)
                        min=i;
                }

                averaj=takim_isaretci_dizisi[min]->attigi_gol-takim_isaretci_dizisi[min]->yedigi_gol;
                puan=takim_isaretci_dizisi[min]->galibiyet_say*3+takim_isaretci_dizisi[min]->beraber_say;
                oyun_say=takim_isaretci_dizisi[min]->beraber_say+takim_isaretci_dizisi[min]->galibiyet_say+takim_isaretci_dizisi[min]->maglubiyet_say;
                // for dongusunden sonra min indeksin altinda en az gol yiyen takim bulunacaktir
                printf("\n TAKIM KODU TAKIM ADI:        OO   GG   BB   MM   AA   YY  AVR  PUAN: \n");
                printf("-----------------------------------------------------------------------\n");
                printf(" %d      %s       %-5d %-5d %-5d %-5d %-5d %-5d %-5d %-5d \n",min,takim_isaretci_dizisi[min]->takim_adi,oyun_say,takim_isaretci_dizisi[min]->galibiyet_say,takim_isaretci_dizisi[min]->beraber_say,takim_isaretci_dizisi[min]->maglubiyet_say,takim_isaretci_dizisi[min]->attigi_gol,takim_isaretci_dizisi[min]->yedigi_gol,averaj,puan);
            }
            else printf("\n hic bir takim bulunamadi.\n");
            break;
        case 10:

            durum=0;
            for(i=1; i<100; i++)
            {
                if(takim_isaretci_dizisi[i]!=NULL )
                {
                    if(takim_isaretci_dizisi[i]->skor!=NULL) // eger takimin skoru bulunduysa...
                    {
                        durum=1;
                        deplasmanda_en_cok[i]=malloc(sizeof(struct skorlar)); // deplasmanda en cok gol atani bulabilmek icin dizi yaratiliyor
                        deplasmanda_en_cok[i]=takim_isaretci_dizisi[i]->skor;
                        gecici2=takim_isaretci_dizisi[i]->skor;

                        while(gecici2!=NULL)
                        {
                            // degerler karsilastiriliyor
                            if(deplasmanda_en_cok[i]->misafir_takimin_gol<gecici2->misafir_takimin_gol)
                                deplasmanda_en_cok[i]=gecici2;
                            gecici2=gecici2->misafir_sonraki;
                        }
                    }
                }
            }
            if(durum==1)
            {
                for(i=1; i<100; i++)
                {
                    if(deplasmanda_en_cok[i]!=NULL) // dizideki ilk bulunan skorun indeksi max olarak aliniyor
                    {
                        max=i;
                        break;
                    }
                }

                for(i=1; i<100; i++)
                {
                    // degerler karsilastiriliyor
                    if(deplasmanda_en_cok[i]!=NULL && deplasmanda_en_cok[i]->misafir_takimin_gol>deplasmanda_en_cok[max]->misafir_takimin_gol)
                        max=i;
                }

                printf("\n TAKIM KODU TAKIM ADI:        OO   GG   BB   MM   AA   YY  AVR  PUAN: \n");
                printf("-----------------------------------------------------------------------\n");
                max=deplasmanda_en_cok[max]->misafir_takimin_kodu;
                averaj=takim_isaretci_dizisi[max]->attigi_gol-takim_isaretci_dizisi[max]->yedigi_gol;
                puan=takim_isaretci_dizisi[max]->galibiyet_say*3+takim_isaretci_dizisi[max]->beraber_say;
                oyun_say=takim_isaretci_dizisi[max]->beraber_say+takim_isaretci_dizisi[max]->galibiyet_say+takim_isaretci_dizisi[max]->maglubiyet_say;

                printf(" %d      %s       %-5d %-5d %-5d %-5d %-5d %-5d %-5d %-5d \n",max,takim_isaretci_dizisi[max]->takim_adi,oyun_say,takim_isaretci_dizisi[max]->galibiyet_say,takim_isaretci_dizisi[max]->beraber_say,takim_isaretci_dizisi[max]->maglubiyet_say,takim_isaretci_dizisi[max]->attigi_gol,takim_isaretci_dizisi[max]->yedigi_gol,averaj,puan);
            }

            break;

        case 11:
            printf("\n Takim Kodu        Takim Adi          Forma No       Ad Soyad        AA");
            printf("\n-----------------------------------------------------------------------------");
            for(i=1; i<100; i++)
            {
                if(takim_isaretci_dizisi[i]!=NULL) // eger takim bulunduysa...
                {
                    if (hash_tablosu[i].sonraki!=&hash_tablosu[i]) // eger bu takimda futbolcular varsa
                    {
                        durum=1;
                        en_cok_gol_atanlar[i]=malloc(sizeof(struct futbolcular)); // her takim icin en cok atan futbolcuyu tutmak icin dizi kullaniliyor
                        en_cok_gol_atanlar[i]=hash_tablosu[i].sonraki;
                        gecici=hash_tablosu[i].sonraki;
                        while(gecici!=&hash_tablosu[i] )
                        {
                            if(gecici->attigi_gol_say>en_cok_gol_atanlar[i]->attigi_gol_say)
                                en_cok_gol_atanlar[i]=gecici;
                            gecici=gecici->sonraki;
                        }
                        strcpy( takim_adi,takim_isaretci_dizisi[en_cok_gol_atanlar[i]->takim_kodu]->takim_adi);

                        printf("\n    %d           %s           %d          %s           %d",en_cok_gol_atanlar[i]->takim_kodu,takim_adi,en_cok_gol_atanlar[i]->forma_no,en_cok_gol_atanlar[i]->ad_soyad,en_cok_gol_atanlar[i]->attigi_gol_say);
                    }
                }
            }

            for(i=1; i<100; i++)
                if(en_cok_gol_atanlar[i]!=NULL) // ligin gol krali bulmak icin ilk futbolcu basta max olarak aliniyor
                {
                    max=i;
                    break;
                }

            for(i=1; i<100; i++)
            {
                // sonraki futbolcu max indeksin altinda bulunan futbolcu ile karsilastiriliyor
                if(en_cok_gol_atanlar[i]!=NULL && en_cok_gol_atanlar[i]->attigi_gol_say>en_cok_gol_atanlar[max]->attigi_gol_say)
                    max=i;
            }

            // for dongusunden sonra en cok gol atan futbolcu bulunmus olur
            if(durum==1)
            {
                strcpy( takim_adi,takim_isaretci_dizisi[en_cok_gol_atanlar[max]->takim_kodu]->takim_adi);

                printf("\n Ligin gol krali:");
                printf("\n Takim Kodu        Takim Adi          Forma No       Ad Soyad        AA");
                printf("\n-----------------------------------------------------------------------------");
                printf("\n    %d        %s         %d          %s          %d",en_cok_gol_atanlar[max]->takim_kodu,takim_adi,en_cok_gol_atanlar[max]->forma_no,en_cok_gol_atanlar[max]->ad_soyad,en_cok_gol_atanlar[max]->attigi_gol_say);
            }
            break;

        }
    }
    while(secim!=12);
    return 0;
}

int sayi_al(int alt,int ust) // belirli aralikta aldiran fonksiyondur
{
    int sayi;
    do
    {
        printf("\n %d ile %d arasindaki sayiyi giriniz:",alt,ust);
        scanf("%d",&sayi);
    }
    while(sayi>ust || sayi<alt);
    return sayi;
}

struct futbolcular *futbolcu_ara(struct futbolcular *liste_basi_ptr, int aranan)//urun listede bulunursa isaretcisi dondurulur, bulunamazsa NULL dondurulur
{
    struct futbolcular *gecici;

    gecici=liste_basi_ptr->sonraki;
    while (gecici!=liste_basi_ptr && gecici->forma_no<=aranan)
    {
        if (gecici->forma_no==aranan) return gecici;
        gecici=gecici->sonraki;
    }
    return NULL;
}

void sirali_ekle(struct futbolcular *liste_basi_ptr, struct futbolcular *yeni)
{
    // forma no ya gore sirali ekle hazir fonksiyonu
    struct futbolcular *onceki, *gecici;

    onceki=liste_basi_ptr;

    gecici=liste_basi_ptr->sonraki;
    while(gecici!=liste_basi_ptr && gecici->forma_no<yeni->forma_no)
    {
        onceki=gecici;
        gecici=gecici->sonraki;
    }

    yeni->sonraki=gecici;
    onceki->sonraki=yeni;
}

struct futbolcular *hashtan_cikar(struct futbolcular *liste_basi_ptr, int cikarilacak)
{
    struct futbolcular *onceki, *gecici;

    onceki=liste_basi_ptr;
    gecici=liste_basi_ptr->sonraki;
    while(gecici!=liste_basi_ptr && gecici->forma_no<cikarilacak)
    {
        onceki=gecici;
        gecici=gecici->sonraki;
    }

    if(gecici==liste_basi_ptr || gecici->forma_no>cikarilacak)
        return NULL;//futbolcu yoksa NULL dondur
    else //futbolcu varsa
    {
        onceki->sonraki=gecici->sonraki;
        return gecici;//ve cikarilan urunun isaretcisini dondur
    }
}

void sirali_ekle_hafta_noya_gore(struct skorlar *liste_basi_ptr,  struct skorlar *yeni,int durum)
{
    // hafta numarasina gore sirali ekleniyor
    struct skorlar *onceki_ev;
    struct skorlar *gecici_ev;
    struct skorlar *onceki_misafir;
    struct skorlar *gecici_misafir;





    if(durum==0)
    {
        // ev sonrakiyi takip eden kisimdir
        onceki_ev=liste_basi_ptr;
        gecici_ev=liste_basi_ptr->ev_sonraki;


        while(gecici_ev!=NULL && gecici_ev->hafta_no<yeni->hafta_no)
        {
            onceki_ev=gecici_ev;
            gecici_ev=gecici_ev->ev_sonraki;
        }

        yeni->ev_sonraki=gecici_ev;
        onceki_ev->ev_sonraki=yeni;
    }
    else
    {
        // misafir sonrakiyi takip eden kisim duruma gore
        onceki_misafir=liste_basi_ptr;
        gecici_misafir=liste_basi_ptr->misafir_sonraki;

        while(gecici_misafir!=NULL && gecici_misafir->hafta_no<yeni->hafta_no)
        {
            onceki_misafir=gecici_misafir;
            gecici_misafir=gecici_misafir->misafir_sonraki;
        }

        yeni->misafir_sonraki=gecici_misafir;
        onceki_misafir->misafir_sonraki=yeni;
    }


}



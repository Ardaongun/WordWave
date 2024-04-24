#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

struct n {
    char kelime[30];
    char ceviri[100];
    int puan;
    struct n* next;
};

typedef struct n node;

void menu(){
    printf("\n");
    printf("------------------------------------\n");
    printf("*                                  *\n");
    printf("*             Menu                 *\n");
    printf("*                                  *\n");
    printf("*        1. Kelime sor             *\n");
    printf("*        2. Kelime ekle            *\n");
    printf("*        3. Kelimeleri Listele     *\n");
    printf("*        4. Cikis yap              *\n");
    printf("*                                  *\n");
    printf("------------------------------------\n");
}

void linked_liste_eleman_ekle(node** root, char satir[]) {
    node* temp;
    node* iter;
    
    char g_kelime[30] = "gecici";
    char g_ceviri[100] = "gecici";
    int g_sayi = 0;
    char* token;

    char satir_copy[130];
    strcpy(satir_copy, satir);

    token = strtok(satir_copy, "-");
    strcpy(g_kelime, token);

    token = strtok(NULL, "+");
    strcpy(g_ceviri, token);

    token = strtok(NULL, "");
    g_sayi = atoi(token);

    temp = (node*)malloc(sizeof(node));
    if (temp == NULL) {
        printf("Bellek tahsisi yapilamadi.");
        exit(1);
    }
    strcpy(temp->kelime, g_kelime);
    strcpy(temp->ceviri, g_ceviri);
    temp->puan = g_sayi;
    temp->next = NULL;

    if (*root == NULL || strcmp(g_kelime, (*root)->kelime) < 0) {
        temp->next = *root;
        *root = temp;
    } else {
        iter = *root;
        while (iter->next != NULL && strcmp(g_kelime, iter->next->kelime) > 0) {
            iter = iter->next;
        }
        temp->next = iter->next;
        iter->next = temp;
    }
}



void listele(node** root){
    printf("\ndebug\n");
    node* iter;
    iter = *root;
    printf("\n----\t\t\t-------Liste-------\t\t\t----\n\n");
    while(iter != NULL){
        printf("%s %s \n",iter->kelime,iter->ceviri);
        iter = iter->next;
    }
    printf("\n");
    sleep(3);
}
void datacek(node** root){
    FILE* op;
    char satir[130];

    op = fopen("kelime_data.txt","r");

    if(op == NULL){
        printf("Kelimelere erisilemedi\n");
    }

    else{
        while(fgets(satir,sizeof(satir),op) != NULL){
            linked_liste_eleman_ekle(root,satir);
        }
    }
    fclose(op);
}
void datayaz(node** root){
    node* iter;
    iter = *root;
    FILE* yp;
    yp = fopen("kelime_data.txt","w");

    while(iter != NULL){
        fprintf(yp,"%s-%s+%d",iter->kelime,iter->ceviri,iter->puan);
        if(iter->next != NULL){
            fprintf(yp,"\n");
        }
        iter = iter->next;
    }
}
void kelime_sil(node** root,char kelime[30]){ // eger ilk kelime silinirse liste kaybedilir sorunu coz
    node* iter;
    node* temp;
    iter = *root;
    if(*root == NULL){
        printf("Silinecek eleman yok\n");
    }
    else if(strcmp(iter->kelime,kelime) == 0){
        *root = iter->next;
        free(iter);
    }
    else{
        while(strcmp(iter->kelime,kelime)){
            temp = iter;
            iter = iter->next;
        }
        temp->next = iter->next;
        free(iter);
    }
}
int linked_listte_kac_eleman_var(node** root){
    node* iter = *root;
    int satir = 0;
    while(iter != NULL){// linked listde kac eleman var 
        iter = iter->next;
        satir++;
    }
    return satir;

}

void sinav_yap(node** root){
    node* iter = *root;
    int satir = linked_listte_kac_eleman_var(root);
    int random;
    char cevap[30];
    char kontrol[30];
    int flag = 0;
    srand(time(NULL));

    iter = *root;
    random = rand() % satir;
    for(int i = 0 ; i < (random - 1)  && iter != NULL; i++ ){
        iter = iter->next;
    }
    printf("%s :",iter->kelime);
    scanf("%s",cevap);
    strcpy(kontrol,iter->ceviri);
    char* token = strtok(kontrol,"-");
    while( token != NULL){
        if(strcmp(token,cevap) == 0){
            flag = 1;
            break;
        }
        token = strtok(NULL,"-");
    }

    if(flag == 1){
        printf("Dogru bildiniz!\n");
        iter->puan += 1;
        if(iter->puan >= 5){
            kelime_sil(root,iter->kelime);
        }
    }

    else{
        printf("Yanlıs! Dogru ceviriler: %s\n",iter->ceviri);
        if(iter->puan > 0){
            iter->puan -= 1;
        }
        
    }
    sleep(1);
}

void kelime_ekle(node** root){
    char kelime[30];
    char satir[130];
    char secim = 'e';
    printf("Lutfen eklemek istediginiz kelimeyi giriniz:");
    scanf("%s",kelime);
    strcpy(satir,kelime);
    while(secim != 'h'){
        printf("Lutfen kelimenin cevirisini giriniz:");
        scanf("%s",kelime);
        strcat(satir,"-");
        strcat(satir,kelime);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Kelimenin baska cevirisi var mı(e - evet, h - hayir):");
        scanf("%c",&secim);
    }
    strcat(satir,"+0");
    linked_liste_eleman_ekle(root,satir);
}


int main(){
    printf("\033[2J");
    printf("\033[H");

    char secim[30];
    node* root = NULL;
    datacek(&root);
    
    while(1){
        menu();
        printf("Lutfen seciminizi giriniz:");
        scanf("%s",secim);

        if(strcmp(secim,"1") == 0){
            sinav_yap(&root);
        }

        else if(strcmp(secim,"2") == 0){
            kelime_ekle(&root);
            datayaz(&root);
        }

        else if(strcmp(secim,"3") == 0){
            listele(&root);
        }

        else if(strcmp(secim,"4") == 0){
            datayaz(&root);
            printf("Program sonlanmistir\n");
            return 0;
        }

        else{
            printf("Lutfen dogru bir girdi giriniz!!!!\n");
            sleep(2);
        }
    }
    datayaz(&root);
}
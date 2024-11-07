#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <locale.h>

int num_jogadores = 0;// variavel que recebe o numero de jogadores
typedef struct 
{
    char valor;
    int cor;
}mina; // struct usada na mina principal, para poder alocar os valores e a cor do jogador

mina **mina_principal = NULL; //mina principal que determina o valor dos diamantes
char **mina_exibida = NULL;// mina que é exibida na tela, que é atualizada constantemente
int pistas[2]; // vetor que armazena as pistas
int cont = 0; // variavel de controle das pistas

typedef struct {
    char nome[50];
    int score;
    int cor_j;
}jogador; // struct que define o jogador
jogador *jogadores = NULL; //vetor de jogadores

void gotoxy(int x, int y){ //Função  para controlar o cursor
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
}

void setColor(int color){// função que define as cores de outputs especificos
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

void inicializarMinas() { //função que aloca memória paras as matrizes das minas
    mina_principal = malloc(6 * sizeof(mina *));
    mina_exibida = malloc(6 * sizeof(char *));
    for (int i = 0; i < 6; i++) {
        mina_principal[i] = malloc(8 * sizeof(mina));
        mina_exibida[i] = malloc(8 * sizeof(char));
    }
}

void liberarMinas() { //função que libera a memória alocadas das matrizes
    for (int i = 0; i < 6; i++) {
        free(mina_principal[i]);
        free(mina_exibida[i]);
    }
    free(mina_principal);
    free(mina_exibida);
}

void menuInicial(){ // função que cria o menu
    int valido = 0; //variavel de controle
    system("cls"); // limpa a tela
   // for's que criam o contorno do menu
    for(int i = 1; i <= 118; i++){ // contorno superior
        gotoxy(i,0);
        printf("-");
        Sleep(1); // intervalo de um milisegundo
    }
    for(int j = 0;j <= 29;j++){ //  lado direito
        gotoxy(118,j);
        printf("|");
        Sleep(1);// intervalo de um milisegundo
    }
    for(int k = 117; k >= 0; k--){ // contorno de baixo
        gotoxy(k,29);
        printf("-");
        Sleep(1);// intervalo de um milisegundo
    }
    for(int l = 29; l >= 0; l--){ // lado esquerdo
        gotoxy(0,l);
        printf("|");
        Sleep(1);// intervalo de um milisegundo
    }

    while(valido == 0){//laço de controle para evitar entrada inválida
        gotoxy(46,12); //criaçãoo do menu com auxilio da funçãoo gotoxy para centralizar ele
        printf("-----MATRIX FEVER-----");
        Sleep(1000); // espera um segundo
        gotoxy(38,13);
        printf("Escolha o Número de Jogadores (2, 3 ou 4): ");
        gotoxy(38,14);
        printf("-> ");
        fflush(stdin);
        scanf("%d",&num_jogadores); // salva a escolha do usuario

        if(num_jogadores > 1 && num_jogadores < 5){// verifica se a entrada do usuário é valida
            valido = 1; //valida a escolha e quebra o loop
        }
        else{
            gotoxy(38,16);
            printf("ESCOLHA INVÁLIDA");//informa o jogador
            Sleep(1500);
            system("cls"); //limpa a tela para repetir o processo
        }

    }
}

void iniJogadores(){ // inicia o vetor de jogadores com valores padrões
    for(int j = 0; j< num_jogadores;j++){
        strcpy(jogadores[j].nome, "NULL");
        jogadores[j].score = 0;
        jogadores[j].cor_j = j+3;
    }
}

void selJogadores(){// menu de seleção de jogadores
    char nome[50]; // varivel temporaria do nome
    system("cls"); //limpa a tela
    gotoxy(46,12);// print localizado
    printf("SELEÇÃO DE JOGADORES");
    
    
    for(int i = 0;i<num_jogadores; i++ ){ // input do nome de cada jogador
        gotoxy(46,13);
        printf("Informe o Nome do Jogador %d-> ",i+1);
        fflush(stdin);
        fgets(nome,50,stdin);
        nome[strcspn(nome, "\n")] = '\0'; // elimina a nova linha do input do nome
        strcpy(jogadores[i].nome,nome); //copia o input da variavel temporaria para o vetor
        system("cls");
    }

    for(int j = 0;j<num_jogadores; j++ ){ // realiza o print do nome dos jogadores e identifica as cores dos jogadores
        gotoxy(20,j+13);
        setColor(jogadores[j].cor_j);
        printf("P%d : %s ",j+1,jogadores[j].nome);
        printf("Essa é sua Cor!");
    }
    Sleep(1500);
}

void limparMina(){ // limpa os tabuleiros, para que as posiçõees das minas possam ser sorteadas corretamente em seguida
    for(int i = 0; i < 6; i++){ // orientação da linha das matrizes
        for(int j = 0; j < 8; j++){// orientaçãoo da coluna das matrizes
            mina_principal[i][j].valor = '0'; // mina principal
            mina_principal[i][j].cor = 2;
            mina_exibida[i][j] = ' '; // mina exibida na tela
        }
    }
}

void plantarDiamantes(){
    int lin,col; // variaveis inteiras para a linha e coluna sorteada para as pistas e armadilhas
    int quilate; // quilate que sera convertido
    srand(time(NULL)); // função para mudar o valor da seed

    for(int i = 0; i < 39; i++){ // for que controla o numero de diamantes sorteadas
        lin = rand() % 6; // sortea a linha
        col = rand() % 8; // sortea a coluna

        if(mina_principal[lin][col].valor == '0'){ // verifica se esta vazia a posição sorteada
            if(i == 0){
                mina_principal[lin][col].valor = '*'; // diamante especial que valerá 20 quilates
                pistas[0] = lin;
                pistas[1] = col;
            }
                
            else{
                quilate = 1 + (rand() % 9);
                mina_principal[lin][col].valor = quilate +'0'; // converte int em char
            }
                
        }
        else{ // se nao regride a variavel do for para repetir o processo
            i--;
        }
    }

}

void plantarEspeciais(){
    int lin,col; // variaveis inteiras para a linha e coluna sorteada para as pistas e armadilhas
    srand(time(NULL)); // função para mudar o valor da seed

    for(int i = 0; i < 6; i++){ // for que controla o numero de armadilhas sorteadas
        lin = rand() % 6; // sortea a linha
        col = rand() % 8; // sortea a coluna

        if(mina_principal[lin][col].valor == '0'){ // verifica se esta vazia a posição sorteada
            if(i<=1)
                mina_principal[lin][col].valor = 'O'; // planta dois "Rasgo na Bolsa"
            else if(i>1 && i<=3)
                mina_principal[lin][col].valor = '+'; // planta dois "Baú Secreto"
            else if(i>3 && i<6)
                mina_principal[lin][col].valor = '-';// planta dois "Sabotagem" 
        }
        else{ // se nao regride a variavel do for para repetir o processo
            i--;
        }
    }

    for(int j = 0;j<2;j++){//sorteio da pista
        lin = rand() % 6;
        col = rand() % 8;

        if(mina_principal[lin][col].valor == '0'){  // verifica se esta vazia a posição sorteada
            mina_principal[lin][col].valor = 'P';
        }
        else // se nao regride a variavel do for para repetir o processo
            j--;
        
    }
}

void mostrarMina() { // função de exibição da mina
    setColor(2);
    gotoxy(41, 5);
    printf(" | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |");  // Cabeçalho de colunas
    gotoxy(41, 6);
    printf("-|---+---+---+---+---+---+---+---|");

    for (int i = 0; i < 6; i++) {  // Loop para cada linha
        gotoxy(41, 7 + 2 * i);
        printf("%d|", i);  // Imprime o número da linha

        for (int j = 0; j < 8; j++) {  // Loop para cada coluna na linha
            setColor(mina_principal[i][j].cor); //seta a cor com base na cor do jogador
            printf(" %c ",mina_exibida[i][j]); // imprime o valor da casa
            setColor(2);
            printf("|");
        }
        
        gotoxy(41, 8 + 2 * i);  // Posição da linha divisória
        printf(" |---+---+---+---+---+---+---+---|");
    }
}

void placar(){ // função que imprime o placar
    gotoxy(5,9);
    setColor(2);
    printf("----PLACAR----"); // exibe o nome e pontuação de cada jogador
    for(int i = 0;i<num_jogadores;i++){
        gotoxy(5,10+i);
        setColor(jogadores[i].cor_j);
        printf("%s: %d",jogadores[i].nome,jogadores[i].score);
    }
}

void somaPontos(int lin, int col,int player){ // função que identifica o valor da casa e soma a pontuação
    gotoxy(80,7);

    if(mina_principal[lin][col].valor == 'O'){ // se for um " rasgo na bolsa" subtrai os quilates
        printf("Você Rasgou sua Bolsa!");
        gotoxy(80,8);
        printf("Que Azar...Perca 10 Quilates!");
        jogadores[player].score -= 10;
        if(jogadores[player].score<0)
            jogadores[player].score = 0;
    }
    else if(mina_principal[lin][col].valor == '+'){// se for um bau soma os quilates
        printf("O que é isso?");
        gotoxy(80,8);
        printf("Você encontrou um Baú Secreto!");
        gotoxy(80,9);
        printf("Ganhe 5 Quilates");
        jogadores[player].score += 5;
    }
    else if(mina_principal[lin][col].valor == '-'){ // se for um sabotagem decresce os quilates dos adversarios
        printf("Ao estilo Dick Vigarista");
        gotoxy(80,8);
        printf("você sabotou seus oponentes!");
        gotoxy(80,9);
        printf("Seus Oponentes perdem 5 quilates");
        for(int i = 0;i<num_jogadores;i++){
            if(i!=player){
                jogadores[i].score-=5;
                if(jogadores[i].score<0)
                    jogadores[i].score = 0;
            }
        }
    }
    else if(mina_principal[lin][col].valor == 'P'){ // revela as pistas
        printf("Você encontrou uma Pista!");
        gotoxy(5,20);
        if(cont == 0){
            printf("Linha da Gema Primordial: %d",pistas[0]);
            cont++;
        }
        else{
            printf("Coluna da Gema Primordial: %d",pistas[1]);
        }
    }
    else if(mina_principal[lin][col].valor == '*'){
        printf("WOW...Você encontrou a Gema Primordial!");
        gotoxy(80,8);
        printf("Você ganhou 20 Quilates!");
        jogadores[player].score += 20;
    }
    else{ // soma os quilates se for diamante
        int num;
        num = mina_principal[lin][col].valor -'0'; //converte de char para int
        printf("Você encontrou %d Quilate(s)!",num);
        jogadores[player].score += num;
    }
    Sleep(2000);
}

void jogar(){ // função principal que controla o fluxo de jogo
    int rodadas = 0,valido,linha,coluna; // variaveis de controle e as que recebem a tentativa do jogador

    for(int vez = 0;vez<num_jogadores;vez++){// for que controla os turnos dos jogadores
        valido = 0;
        while(valido ==0){//controla se a escolha é válida
            system("cls");//limpa a tela
            //identifica o turno do jogador
            setColor(jogadores[vez].cor_j); 
            gotoxy(5,5);
            printf("Jogador %d: %s\n",vez+1,jogadores[vez].nome);

            placar();//chama a função de exibição do placar
            mostrarMina(); //chama a função de exibição das minas
            gotoxy(38,24); // exibe a mensagem pedindo a coordenada da tentativa na linha correta
            printf("Digite a Coordenada do seu chute (Formato Linha Coluna): ");
            fflush(stdin); //limpa o fluxo
            scanf("%d%d",&linha,&coluna);//Recebe a tenativa do jogador

            if(linha < 0 || linha > 5 || coluna < 0 || coluna > 7 || mina_exibida[linha][coluna] != ' '){
                gotoxy(38,26);
                printf("ESCOLHA UMA CASA VÁLIDA!!!");
                Sleep(1500);
            }
            else
                valido = 1;
        }
        
        mina_exibida[linha][coluna]= mina_principal[linha][coluna].valor; // associa o valor da matriz principal, a matriz de exibição
        mina_principal[linha][coluna].cor = jogadores[vez].cor_j; // associa a cor do caractere a cor do jogador

        somaPontos(linha,coluna,vez);// chama a função de somatorio de pontos

        if(vez+1 == num_jogadores){ // soma as rodadas ao final das jogadas 
            rodadas++;
        }
        if(vez+1 == num_jogadores  && rodadas<(48/num_jogadores)){ // reinicia a variavel caso não tenha encerrado o numero de rodadas
            vez = -1;
        }
    }


}

void resultado(){ // função que verifica o resultado
    int vencedor,maior = 0;
    system("cls");
    for(int i = 0;i<num_jogadores;i++){
        if(jogadores[i].score>maior){
            maior = jogadores[i].score;
            vencedor = i;
        }
    }
    gotoxy(46,12);
    printf("Fim de Jogo!");
    gotoxy(46,13);
    printf("O vencedor foi: %s com %d pontos!",jogadores[vencedor].nome,jogadores[vencedor].score);
    gotoxy(46,14);
    system("pause");//evita que o programa feche sozinho, esperando ate o usario clique em alguma tecla
}

int main(){
    //configurações iniciais
    system("color 02");
    SetConsoleOutputCP(CP_UTF8);

    inicializarMinas();//aloca a memoria das matrizes
    menuInicial();// seleção de numero de jogadores
    jogadores = (jogador *)malloc(num_jogadores * sizeof(jogador));//alocação do vetor de jogadores

    iniJogadores();//incialização dos vetores e depois seleção de nomes
    selJogadores();
    //limpa as matrizes e plantas o diamantes e armadilhas/pistas
    limparMina();
    plantarDiamantes();
    plantarEspeciais();

    jogar();// função principal da partida
    resultado();//verifica quem ganhou ao final

    //libera os ponteiros
    liberarMinas();
    free(jogadores);
    return 0;
}

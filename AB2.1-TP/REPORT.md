# Identificação

* Página do repositório do trabalho [link do repositório no github](https://github.com/Clemensss/teaching)

* Discente 1
  * Nome: Clemens August Souto Schrage
  * Matrícula:
* Discente 2
  * Nome: João Victor Falcão Santos Lima
  * Matrícula: 19111156
* Discente 3
  * Nome: Rodrigo Santos da Silva
  * Matrícula:

# Resultados

## Uso intenso da UCP

![Alt text](https://media.discordapp.net/attachments/443576706775777313/614579876128751617/Uso_Intenso_da_UCP.png?width=683&height=456)

## Uso intenso da Memória

![Alt text](https://media.discordapp.net/attachments/443576706775777313/614584227907764224/Uso_Intenso_da_Memoria.png?width=728&height=455)

# Discussão

Em ambos os testes, a fim de monitorar os recursos utilizados pelo processo filho, foi utilizada a seguinte sequência de comandos concatenados:

```bash
    ps -e -o pid,rss,pcpu | grep $pid | awk '{ printf "%-10s%-5s\n", $2, $3 }'
```

O comando `ps` é o responsável por informar os processos ativos no momento em que o mesmo foi invocado. Por meio desse comando e de suas flags `-e`, responsável por listar todos os processos presentes no sistema, e `-o`, responsável por formatar a saída do comando é possível listar o **pid** (número de identificação do processo), o **rss** (o quanto de memória o processo usufrui da memória RAM) e **pcpu** (o percentual médio de uso do processador ao longo da execução do processo). Com o `grep`, é possível filtrar o resultado para exibir apenas as saídas referentes ao **pid** determinado, enquanto o `awk` seleciona e imprime os valores dentro dos campos `$2` e `$3` - a utilização de memória e UCP, respectivamente.

## Uso intenso da UCP

Observando os dados obtidos nos testes, pode-se notar que o uso da UCP, em muitos casos, ultrapassa 100%. Primeiramente esse resultado foi inesperado e contra intuitivo. Contudo, ao verificarmos a especificação do `ps`, vimos que a estimativa de uso da UCP informada pelo comando é relativa a **um núcleo** do processador (desse modo, caso o processador tenha, por exemplo, 6 núcleos e o processo esteja utilizando 100% de cada um deles, será informado um total de 600%). Assim, chegamos à conclusão que o fato observado ocorre quando o processo demanda o uso de mais de um dos núcleos do processador.

```C
    for(;;)
    {
    }
```

Continuando a análise dos resultados, percebeu-se que o gráfico de uso da UCP apresenta um crescimento muito rápido nos primeiros segundos, e em seguida se mantém próximo aos 100%. Isso ocorre pelo fato do trecho de código utilizado ser um loop infinito mostrado acima), o qual monopoliza o tempo de uso do processador. Essa monopolização ocorre mesmo com o escalonamento de tarefas do sistema operacional sendo preemptivo, pois durante o laço infinito não são realizadas chamadas de sistemas que interrompam o fluxo de execução do processo, o qual se mantém sempre “pronto” para receber tempo de UCP novamente e continuar suas iterações.

Entretanto, graças à política de **tempo compartilhado** aplicada pelos sistemas operacionais atuais, os outros processos continuam a receber suas fatias de tempo de UCP, então o processo pai também continua em execução e a partir dele é possível realizar uma chamada de sistema (código abaixo) para finalizar o processo filho.

```C
    strcpy(bash_cmd, "kill ");
    strcat(bash_cmd, str_pid);
    system(bash_cmd);
```

Vale ressaltar que não ocorreram travamentos ou redução notável no desempenho do sistema operacional. Isso é possível por conta do já mencionado tempo compartilhado, além dos avanços na arquitetura de hardware que possibilitaram processadores com vários núcleos.

## Uso intenso da Memória e da UCP

Para os testes de uso intenso da UCP e da memória, inicialmente foi proposto o código abaixo, onde o laço de repetição infinito - como explicado anteriormente - é responsável por consumir o recurso do processador, enquanto a função `malloc()`, definida na biblioteca padrão da linguagem C `<stdlib.h>`, é chamada para alocar blocos de memória dinamicamente. No caso implementado inicialmente, 1 Byte era alocado a cada iteração do loop.

```C
    for(;;)
    {
        malloc(1);
    }
```

Contudo, a alocação ocorreu de modo tão intenso que o processo consumiu toda a memória disponível antes dos 10 segundos de monitoramento propostos serem finalizados. Isso causou travamentos no sistema operacional (e em alguns casos o computador chegou a desligar) pois não havia memória principal disponível para outras funções do sistema.

Então, foi incluída uma estrutura condicional `if (i % 15 == 0)` dentro do laço de repetição responsável por realizar esse teste, com o objetivo de reduzir em 15 vezes a frequência que a função `malloc()` seria chamada, diminuindo assim a quantidade total de memória principal consumida durante a execução do programa. O código ficou como segue:

```C
    int i;
    for(i = 0;;i++)
    {
        if (i % 15 == 0)
        {
            malloc(1);
        }
    }
```

Com essa alteração, o programa pôde ser executado sem causar travamentos no sistema e os testes ocorreram sem mais problemas. Os resultados obtidos no gráfico de uso mostraram um crescimento linear no uso da memória. Isto está de acordo com o esperado, visto que a taxa de alocação de memória é constante (1 Byte alocado a cada 15 iterações do laço de repetição).

Era esperada uma redução no uso da UCP devido às interrupções causadas por várias chamadas de sistema para alocar memória, mas na prática o consumo desse recurso manteve-se bastante elevado. Essa queda de consumo não aconteceu por conta da limitação em código das vezes em que a função `malloc()` seria chamada, além da eficiência da linguagem C, a qual não possui um nível muito alto de abstração e é a mesma linguagem em que grande parte dos sistemas operacionais são desenvolvidos, portanto possui um grande desempenho.

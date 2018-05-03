#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#define ALPHABET_SIZE 80

struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
};
wchar_t *Sugt[5];
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode =( struct TrieNode *)malloc(sizeof(struct TrieNode));
    pNode->isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;
    return pNode;
}
void insert(struct TrieNode *root, wchar_t *key)
{

    struct TrieNode *pCrawl = root;
    int i,index;
    for ( i = 0; i < wcslen(key); i++)
    {
       index =(int)( key[i] - 0x0c82);
        if (pCrawl->children[index] == NULL)
            pCrawl->children[index] = getNode();
        pCrawl = pCrawl->children[index];
    }
    // mark last node as leaf
    pCrawl->isEndOfWord = true;
}
void suggest(struct TrieNode* root, wchar_t *word, wchar_t *Sugt[])
{
    int i = 0, j = 0, k = 0, index, len, flag = 0;
    int pos = -1;
    int wCount = 0;
    wchar_t *W, letter;
    struct TrieNode *r = root, *temp, *temp2, *temp3;
    while(word[i] != '\0')
    {
        index = (int)(word[i++] -0x0c82);
        if(r -> children[index] == NULL)
        {
            printf("No Suggestions!");
            return;
        }
        r = r -> children[index];
    }
    wCount = 1;
    while(j < ALPHABET_SIZE && wCount < 5)
    {
        temp2 = temp = r -> children[j];
        len = wcslen(word);
        W = word;  
        i = 0;
        if(temp == NULL)
        {
            j++;
            continue;
        }
        letter = (wchar_t)(j + 0x0c82);
        W[len++] = letter;
        if(temp -> isEndOfWord == true)
        {            
            W[len] = '\0';
            printf("\n\t%d. %ls", ++wCount, W);
            Sugt[wCount] = W;
            flag = 1;
            j++;
            continue;
        }
        LOOP:
        pos = -1;
        k = 0;
        while(k < ALPHABET_SIZE)
        {
            if(temp2 -> children[k] != NULL)
                {
                    pos = k;
                    flag = 1;
                    break;
                }
            k++;
        }
        int flag2 = 0;
        k = 0;
        while(k < ALPHABET_SIZE)
        {            
            temp3 = temp2 -> children[k];
            if(temp3 == NULL)
            {
                k++;
                continue;
            }
            if(temp3 -> isEndOfWord == true)
            {
                letter = (wchar_t)(k + 0x0c82);
                W[len++] = letter;
                W[len] = '\0';
                printf("\n\t%d. %ls", ++wCount, W);
                Sugt[wCount] = W;
                flag = 1;
                flag2 = 1;  
                break;
            } 
            k++;               
        }
        if(flag2 == 0)
        {
            W[len++] = (wchar_t)(pos +0x0c82);
            temp2 = temp2 -> children[pos];
            goto LOOP;
        }
        j++;   
    }
    return;
}
void AutoComplete(struct TrieNode* root)
{
    wchar_t word[20];
    int i;
    printf("Enter word: ");
    scanf("%ls", word);
    printf("\nSuggestions for \"%ls\":\n\n", word);
    suggest(root, word, Sugt);
    printf("\n");     
    return;
}
int main(void)
{
    wchar_t s[200];
    int i;
    FILE *fp;
    fp = fopen("kn_IN.dic","r");
    setlocale(LC_ALL,"");
    struct TrieNode *root = getNode();
    int countf=0;
    while(fscanf(fp,"%ls",s)!=0)
    {
        countf++;
        insert(root,s);
        if(countf==700)
	break;
    }
    AutoComplete(root);
    return 0;
}

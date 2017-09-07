#include <stdio.h>
#include <stdint.h>

const int BLOCK_SIZE = 512;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }
    
    char *infile = argv[1];
    uint8_t buffer[BLOCK_SIZE];
    int counter = 0;
    
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    FILE *img = NULL;
    
    while (fread(&buffer, BLOCK_SIZE, 1, inptr) == 1)
    {
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (img != NULL) //如果img已經開始寫了,就關掉img,因為進到這個condition表示又是下一個jpeg的開頭了
            {
                fclose(img);
            }
            
            char filename[8]; //宣告一個字串存檔案名稱
            sprintf(filename, "%03i.jpg", counter); //幫檔案名稱編號
            
            img = fopen(filename, "w");
            
            counter++;
        }
        
        if (img != NULL)
        {
            fwrite(&buffer, BLOCK_SIZE, 1, img);
        }
    }
    
    if (img != NULL)
    {
        fclose(img);
    }
    
    fclose(inptr);
    return 0;
    

    
}
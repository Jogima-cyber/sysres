//
// Implement basic functions on labels
//
//#include "string.c"

void labecpy(label dst,label src)
{
    int i,j;

    for (i=0;i<=3;i++)
    {
        for (j=0;j<=7;j++)
        {
            strncpy(dst[i][j],src[i][j],UIDSIZE);
        }
    }
}

void owners(char o[4][UIDSIZE], label l)
{   
    int i;

    for(i=0; i<=3;i++)
        {
            strncpy(o[i],l[i][0],UIDSIZE);
        }
}

void effective_readers(char r[7][UIDSIZE], label l)
{
    int i,j,k;
    int b;

    for (j=0; j<=6; j++)
    {
        strncpy(r[j],l[0][1+j],UIDSIZE);
    };
    for(i=1; i<=3; i++)
        {
            if (strcmp(l[i][0],"") != 0)
            {
                for (j=0; j<=6; j++)
                {   
                    b = 1;
                    for (k=1; k<=7; k++)
                    {
                        if (strcmp(l[i][k],r[j]) == 0)
                            b = 0;
                    };
                    if (b)
                        strncpy(r[j],"",UIDSIZE);
                }
            }
        }
}

int is_restriction(label l1, label l2)
{
    int b,c = 1;
    int i,j,k,l;
    char o1[4][UIDSIZE];
    char o2[4][UIDSIZE];

    owners(o1, l1);
    owners(o2, l2);

    for (i=0;i<=3;i++)
    {
        c = 1;
        for (j=0;j<=3;j++)
        {
            if (o1[i] == o2[j])
                c = 0;
        };
        if (c)
            b = 0;
    };

    if (!b)
        return b;

    for (i=0;i<=3;i++)
    {
        for (j=0;j<=3;j++)
        {
            if (l1[i][0] == l2[j][0])
                {
                    for (k=1;k<=7;k++)
                    {
                        if (!l1[i][k])
                        {
                            c = 1;
                            for (l=1;l<=7;l++)
                            {
                                if (l1[i][k] == l2[j][l])
                                    c = 0;
                            };
                            if (c)
                                b = 0;
                        }
                    }
                }
        }
    };
    return b;   
}

void join_labels(label l1, label l2, label l)
{
    int i,j,k,m;
    int b = 1;
    int c = 1;

    for (i=0;i<=3;i++)
    {
        c = 1;
        for (j=0;j<=3;j++)
        {
            if (l1[i][0] == l2[j][0])
            {
                c = 0;
                for (k=1;k<=7;k++)
                {
                    b = 1;                    
                    strncpy(l1[i][k],l[i][k],UIDSIZE);
                    for (m=1;m<=7;m++)
                    {
                        if (l1[i][k] == l2[j][m])
                            b = 0;
                    };
                    if (b)
                        strncpy(l[i][k],"",UIDSIZE); 
                }
            }
        };
        if (c)
        {
            for (k=1;k<=7;k++)
            { 
                strncpy(l1[i][k],l[i][k],UIDSIZE);
            }
        }
    };
    for (i=0;i<=3;i++)
    {   
        c = 1;
        for (j=0;j<=3;j++)
        {
            if (l2[i][0] == l1[j][0])
                c = 0;
        };
        if (c)
        {
            b = 1;
            for (j=0;j<=3;k++)
                {
                    if (!l[j])
                    {
                        b = 0;
                        for (k=0;k<=7;k++)
                        {
                            strncpy(l2[i][k],l[j][k],UIDSIZE);
                        }
                    }                   
                };
            if (b)
                b=0;//panic("Too many owners");
        }
    }    
}

// System calls to implement :
    // Declassification : restriction
    // Declassification : add readers
    // Declassification : remove owner
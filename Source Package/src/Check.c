#include "Check.h"
#include "Move.h"
#include "IsValid.h"
int IsCheck(int board[8][8], int turn)
{
	//printf("Now in IsCheck");
	int Tempboard[8][8];
	int KingPosi_x=100;
	int KingPosi_y=100;
	int x, y;
	int move1[5];
	int CheckingPosi_x=100;
	int CheckingPosi_y=100;
	int Start_x;
	int End_x;
	int Start_y;
	int End_y;
	//int i,j; (commented out by Justin, unused variable
	/*initialize the Tempboard*/
	for (x=0;x<8;x++)
	{
		for(y=0;y<8;y++)
		{
			Tempboard[y][x]=board[y][x];
		}
	}
	
	
	
	if (turn%2==0)
	{
	/*White Side*/
	/*Find the king position and the checking position, if there is no piece checking the king, return 0*/
		for(x=0;x<8;x++)
		{
			for(y=0;y<8;y++)
			{
				if(board[y][x]==5)
				{
					KingPosi_x=x;
					KingPosi_y=y;
				}
			}
		}
		//printf("King=%d,%d",KingPosi_x,KingPosi_y);
		if(KingPosi_x==100 && KingPosi_y==100)
		{
			return 2;
		}
		for(x=0;x<8;x++)
		{
			for(y=0;y<8;y++)
			{
				if(board[y][x]==7 || board[y][x]==8 || board[y][x]==9 || board[y][x]==10 || board[y][x]==12)
				{
					move1[0]=x;
					move1[1]=y;
					move1[2]=KingPosi_x;
					move1[3]=KingPosi_y;
					move1[4]=-1;
					/*printf("%d",IsValid(1,board , move1));*/
					if(IsValid(1,board , move1))
					{
						CheckingPosi_x=x;
						CheckingPosi_y=y;
					}
					
				}
				
			}
		}
		if(CheckingPosi_x==100 && CheckingPosi_y==100)
		{
			return 0;
		}
		
		/*see if there is any possible move for the king to escape from the check. if yes, return 1;*/
		for(x=KingPosi_x-1;x<=KingPosi_x+1;x++)
		{
			for(y=KingPosi_y-1;y<=KingPosi_y+1;y++)
			{
				if((x<0 || x>7 || y<0 || y>7) || (x==KingPosi_x && y==KingPosi_y)) 
				{
					continue;
				}
				if(board[y][x]!=1 && board[y][x]!=2 && board[y][x]!=3 && board[y][x]!=4 && board[y][x]!=6)
				{
					move1[0]=KingPosi_x;
					move1[1]=KingPosi_y;
					move1[2]=x;
					move1[3]=y;
					move1[4]=-1;
					Move(move1,Tempboard);
					if(whitecheck(Tempboard,x,y)==0)
					{
						//printf("King can escape,move1[0]=%d,move1[1]=%d,move1[2]=%d,move1[3]=%d,board=%d\n",move1[0],move1[1],move1[2],move1[3],board[y][x]);
						return 1;
					}
					for (int k=0;k<8;k++)
					{
						for(int l=0;l<8;l++)
						{
							Tempboard[l][k]=board[l][k];
						}
					}
				}
				
				
			}
		}
		//printf("King cant move, ally?\n");
		int i,j;
	/*see if any allied piece can block or capture the piece that is checking the king. if yes, return 1*/
		if (board[CheckingPosi_y][CheckingPosi_x]==12 || board[CheckingPosi_y][CheckingPosi_x]==8 || board[CheckingPosi_y][CheckingPosi_x]==10)
		{
			//printf("QRB\n");
			Start_x=(CheckingPosi_x<KingPosi_x)?CheckingPosi_x:KingPosi_x;
			End_x=(CheckingPosi_x<KingPosi_x)?KingPosi_x:CheckingPosi_x;
			Start_y=(CheckingPosi_y<KingPosi_y)?CheckingPosi_y:KingPosi_y;
			End_y=(CheckingPosi_y<KingPosi_y)?KingPosi_y:CheckingPosi_y;
			for(x=0;x<8;x++)
			{
				for(y=0;y<8;y++)
				{
					if(board[y][x]==1 || board[y][x]==2 || board[y][x]==3 || board[y][x]==4 || board[y][x]==6)
					{
						if(((CheckingPosi_x<KingPosi_x) && (CheckingPosi_y>KingPosi_y)) ||((CheckingPosi_x>KingPosi_x) &&(CheckingPosi_y<KingPosi_y)))
						{
							for (i=Start_x,j=End_y;(i<End_x+1)&&(j>Start_y-1);i++,j--)
							{
								//printf("x=%d,y=%d,i=%d,j=%d\n",x,y,i,j);
								move1[0]=x;
								move1[1]=y;
								move1[2]=i;
								move1[3]=j;
								move1[4]=-1;
								if(IsValid(2,board , move1))
								{
									if(x==7 && y==5)
									{
										//printf("Queen?=%d,x=%d,y=%d,i=%d,j=%d",IsValid(2,board , move1),x,y,i,j);
									}
									Move(move1,Tempboard);
									if(whitecheck(Tempboard,KingPosi_x,KingPosi_y)==0)
									{
										//printf("allied can block,%d,%d,%d,%d",move1[0],move1[1],move1[2],move1[3]);
										return 1;
									}
									for (int q=0;q<8;q++)
									{
										for(int v=0;v<8;v++)
										{
											Tempboard[q][v]=board[q][v];
										}
									}
								}
								if(Start_x==End_x)
								{
									i=i-1;
								}
								if(Start_y==End_y)
								{
									j=j+1;
								}
								
							}
							
						}
						else
						{
							
							for (i=Start_x,j=Start_y;(i<End_x+1)&&(j<End_y+1);i++,j++)
							{
								//printf("x=%d,y=%d,i=%d,j=%d\n",x,y,i,j);
								move1[0]=x;
								move1[1]=y;
								move1[2]=i;
								move1[3]=j;
								move1[4]=-1;
								if(IsValid(2,board , move1))
								{
									if(x==7 && y==5)
									{
										//printf("Queen?=%d,x=%d,y=%d,i=%d,j=%d",IsValid(2,board , move1),x,y,i,j);
									}
									Move(move1,Tempboard);
									if(whitecheck(Tempboard,KingPosi_x,KingPosi_y)==0)
									{
										//printf("allied can block,%d,%d,%d,%d",move1[0],move1[1],move1[2],move1[3]);
										return 1;
									}
									for (int q=0;q<8;q++)
									{
										for(int v=0;v<8;v++)
										{
											Tempboard[q][v]=board[q][v];
										}
									}
								}
								if(Start_x==End_x)
								{
									i=i-1;
								}
								if(Start_y==End_y)
								{
									j=j-1;
								}
								
							}
						}
					}
				}
			}
		}
		else
		{
			//printf("other\n");
			for(x=0;x<8;x++)
			{
				for(y=0;y<8;y++)
				{
					move1[0]=x;
					move1[1]=y;
					move1[2]=CheckingPosi_x;
					move1[3]=CheckingPosi_y;
					move1[4]=-1;
					if(IsValid(2,board , move1))
					{
						Move(move1,Tempboard);
						if(whitecheck(Tempboard,KingPosi_x,KingPosi_y)==0)
						{
							//printf("allied can block,%d,%d,%d,%d",move1[0],move1[1],move1[2],move1[3]);
							return 1;
						}
						for (int q=0;q<8;q++)
						{
							for(int v=0;v<8;v++)
							{
								Tempboard[q][v]=board[q][v];
							}
						}
					}
				}
			}
			
		
		
		}
		
		return 2;
		
	
	
	}
	else
	{
	/*Black Side
	Find the king position and the checking position, if there is no piece checking the king, return 0*/
		for(x=0;x<8;x++)
		{
			for(y=0;y<8;y++)
			{
				if(board[y][x]==11)
				{
					KingPosi_x=x;
					KingPosi_y=y;
				}
				
			}
		}
		if(KingPosi_x==100 && KingPosi_y==100)
		{
			return 2;
		}
		//printf("king posi found");
		for(x=0;x<8;x++)
		{
			for(y=0;y<8;y++)
			{
				if(board[y][x]==1 || board[y][x]==2 || board[y][x]==3 || board[y][x]==4 || board[y][x]==6)
				{
					move1[0]=x;
					move1[1]=y;
					move1[2]=KingPosi_x;
					move1[3]=KingPosi_y;
					move1[4]=-1;
					/*printf("%d",IsValid(2,board , move1));*/
					if(IsValid(2,board , move1))
					{
						
						CheckingPosi_x=x;
						CheckingPosi_y=y;
						
					}
					
				}
			
			}
		}
		if(CheckingPosi_x==100 && CheckingPosi_y==100)
		{
			
			return 0;
		}
		
		/* see if there is any possible move for the king to escape from the check. if yes, return 1;*/
		for(x=KingPosi_x-1;x<=KingPosi_x+1;x++)
		{
			for(y=KingPosi_y-1;y<=KingPosi_y+1;y++)
			{
				if((x<0 || x>7 || y<0 || y>7) || (x==KingPosi_x && y==KingPosi_y)) 
				{
					continue;
				}
				
				if(board[y][x]!=7 && board[y][x]!=8 && board[y][x]!=9 && board[y][x]!=10 && board[y][x]!=12)
				{
					move1[0]=KingPosi_x;
					move1[1]=KingPosi_y;
					move1[2]=x;
					move1[3]=y;
					move1[4]=-1;
					Move(move1,Tempboard);
					if(blackcheck(Tempboard,x,y)==0)
					{
						return 1;
					}
					for (int i=0;i<8;i++)
					{
						for(int j=0;j<8;j++)
						{
							Tempboard[i][j]=board[i][j];
						}
					}
				}
				
			}
		}
		//printf("allied piece block???");
		int i,j;
		/*see if any allied piece can block or capture the piece that is checking the king. if yes, return 1*/
		if (board[CheckingPosi_y][CheckingPosi_x]==6 || board[CheckingPosi_y][CheckingPosi_x]==2 || board[CheckingPosi_y][CheckingPosi_x]==4)
		{
			Start_x=(CheckingPosi_x<KingPosi_x)?CheckingPosi_x:KingPosi_x;
			End_x=(CheckingPosi_x<KingPosi_x)?KingPosi_x:CheckingPosi_x;
			Start_y=(CheckingPosi_y<KingPosi_y)?CheckingPosi_y:KingPosi_y;
			End_y=(CheckingPosi_y<KingPosi_y)?KingPosi_y:CheckingPosi_y;
			for(x=0;x<8;x++)
			{
				for(y=0;y<8;y++)
				{
					if(board[y][x]==12 || board[y][x]==7 || board[y][x]==8 || board[y][x]==9 || board[y][x]==10)
					{
						if(((CheckingPosi_x<KingPosi_x) && (CheckingPosi_y>KingPosi_y)) ||((CheckingPosi_x>KingPosi_x) &&(CheckingPosi_y<KingPosi_y)))
						{
							for (i=Start_x,j=End_y;(i<End_x+1)&&(j>Start_y-1);i++,j--)
							{
								//printf("x=%d,y=%d,i=%d,j=%d\n",x,y,i,j);
								move1[0]=x;
								move1[1]=y;
								move1[2]=i;
								move1[3]=j;
								move1[4]=-1;
								if(IsValid(1,board , move1))
								{
									Move(move1,Tempboard);
									if(blackcheck(Tempboard,KingPosi_x,KingPosi_y)==0)
									{
										return 1;
									}
									for (int q=0;q<8;q++)
									{
										for(int v=0;v<8;v++)
										{
											Tempboard[q][v]=board[q][v];
										}
									}
								}
								if(Start_x==End_x)
								{
									i=i-1;
								}
								if(Start_y==End_y)
								{
									j=j+1;
								}
								
							}
						}
						else
						{
							for (i=Start_x,j=Start_y;(i<End_x+1)&&(j<End_y+1);i++,j++)
							{
								move1[0]=x;
								move1[1]=y;
								move1[2]=i;
								move1[3]=j;
								move1[4]=-1;
								if(IsValid(1,board , move1))
								{
									Move(move1,Tempboard);
									if(blackcheck(Tempboard,KingPosi_x,KingPosi_y)==0)
									{
										return 1;
									}
									for (int q=0;q<8;q++)
									{
										for(int v=0;v<8;v++)
										{
											Tempboard[q][v]=board[q][v];
										}
									}
								}
								if(Start_x==End_x)
								{
									i=i-1;
								}
								if(Start_y==End_y)
								{
									j=j-1;
								}
								
							}
						}
					}
				}
			}
		}
		else
		{
			for(x=0;x<8;x++)
			{
				for(y=0;y<8;y++)
				{
					if(board[y][x]==7 || board[y][x]==8 || board[y][x]==9 || board[y][x]==10 || board[y][x]==12)
					{
						move1[0]=x;
						move1[1]=y;
						move1[2]=CheckingPosi_x;
						move1[3]=CheckingPosi_y;
						move1[4]=-1;
						if(IsValid(1,board , move1))
						{
							Move(move1,Tempboard);
							if(blackcheck(Tempboard,KingPosi_x,KingPosi_y)==0)
							{
								return 1;
							}
							for (int q=0;q<8;q++)
							{
								for(int v=0;v<8;v++)
								{
									Tempboard[q][v]=board[q][v];
								}
							}
						}
					}
					
				}
			}
		}
		return 2;
		
		
		
		
		
		
		
		
		
	}
	
}
int whitecheck(int board[8][8],int KingPosi_x, int KingPosi_y)
{
	int move1[5];
	int Check=0;
	int x,y;
	for(x=0;x<8;x++)
	{
		for(y=0;y<8;y++)
		{
			if(board[y][x]==7 || board[y][x]==8 || board[y][x]==9 || board[y][x]==10 || board[y][x]==12)
			{
				move1[0]=x;
				move1[1]=y;
				move1[2]=KingPosi_x;
				move1[3]=KingPosi_y;
				move1[4]=-1;
				if(IsValid(1,board , move1))
				{
					Check=1;
				}
			}
				
		}
	}
	return Check;
}
int blackcheck(int board[8][8],int KingPosi_x, int KingPosi_y)
{
	int move1[5];
	int Check=0;
	int x,y;
	for(x=0;x<8;x++)
	{
		for(y=0;y<8;y++)
		{
			if(board[y][x]==1 || board[y][x]==2 || board[y][x]==3 || board[y][x]==4 || board[y][x]==6)
			{
				move1[0]=x;
				move1[1]=y;
				move1[2]=KingPosi_x;
				move1[3]=KingPosi_y;
				move1[4]=-1;
				if(IsValid(2,board , move1))
				{
					Check=1;
				}
			}	
		}
	}
	return Check;
}

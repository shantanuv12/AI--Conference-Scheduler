/*
 * File:   SessionOrganizer.cpp
 * Author: Shantanu Verma, Kartik Kumar
 *
 */

#include "SessionOrganizer.h"
#include "Util.h"

SessionOrganizer::SessionOrganizer ( )
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
    processingTimeInMinutes = 0;
    tradeoffCoefficient = 1.0;
}

SessionOrganizer::SessionOrganizer ( string filename )
{
    readInInputFile ( filename );
    conference = new Conference ( parallelTracks, sessionsInTrack, papersInSession );
}

void SessionOrganizer::SwapPapers(int t1, int p1, int k1, int t2, int p2, int k2)
{
  int id1 = conference->getTrack(t1).getSession(p1).getPaper(k1);
  int id2 = conference->getTrack(t2).getSession(p2).getPaper(k2);
  conference->setPaper(t1,p1,k1,id2);
  conference->setPaper(t2,p2,k2,id1);
}

void SessionOrganizer::Randomizer()
{
  srand(time(NULL));
  int p = conference->getSessionsInTrack();
  int t = conference->getParallelTracks();
  int k = conference->getPapersInSession();
  int elements = (t * p * k)/4;
  for(int i = 0 ; i < elements ; i++){
    int x1 = rand() % t;
    int x2 = rand() % t;
    int y1 = rand() % p;
    int y2 = rand() % p;
    int z1 = rand() % k;
    int z2 = rand() % k;
    SwapPapers(x1,y1,z1,x2,y2,z2);
  }
}

double SessionOrganizer::updateScore(double score,int x1, int y1, int z1, int x2,int y2, int z2)
{
  Track temp1 = conference->getTrack(x1);
  Track temp2 = conference->getTrack(x2);
  Session s1 = temp1.getSession(y1);
  Session s2 = temp2.getSession(y2);
  int pid1 = s1.getPaper(z1);
  int pid2 = s2.getPaper(z2);
  for(int i = 0 ; i < s1.getNumberOfPapers() ; i++)
  {
    score = score - (1-distanceMatrix[pid1][s1.getPaper(i)]) + (1-distanceMatrix[pid2][s1.getPaper(i)]) + tradeoffCoefficient * (distanceMatrix[pid1][s1.getPaper(i)]) - tradeoffCoefficient * (distanceMatrix[pid2][s1.getPaper(i)]);
  }
  for(int i = 0 ; i < s2.getNumberOfPapers() ; i++)
  {
    score = score - (1-distanceMatrix[pid2][s2.getPaper(i)]) + (1- distanceMatrix[pid1][s2.getPaper(i)]) + tradeoffCoefficient * (distanceMatrix[pid2][s2.getPaper(i)]) - tradeoffCoefficient * (distanceMatrix[pid1][s2.getPaper(i)]);
  }
  /*for(int i=0;i<numberOfPaperz;i++){
    score = score-(1-distanceMatrix[pid1][s1.getPaper(i)])+(1-distanceMatrix[pid2][s1.getPaper(i)]);
  }
  for(int i=0;i<numberOfPaperz;i++){
    score = score-(1-distanceMatrix[pid2][s2.getPaper(i)])+(1-distanceMatrix[pid1][s2.getPaper(i)]);
  }
  for(int i=0;i<numberOfPaperz;i++){
    score = score-distanceMatrix[pid2][s1.getPaper(i)]+distanceMatrix[pid1][s1.getPaper(i)];
  }
  for(int i=0;i<numberOfPaperz;i++){
    score = score-distanceMatrix[pid1][s2.getPaper(i)]+distanceMatrix[pid2][s2.getPaper(i)];
  }*/
  return score;
}

void SessionOrganizer::organizePapers ( )
{
    int paperCounter = 0;
    for ( int i = 0; i < conference->getSessionsInTrack ( ); i++ )
    {
        for ( int j = 0; j < conference->getParallelTracks ( ); j++ )
        {
            for ( int k = 0; k < conference->getPapersInSession ( ); k++ )
            {
                conference->setPaper ( j, i, k, paperCounter );
                paperCounter++;
            }
        }
    }
    int p = conference->getSessionsInTrack();
    int t = conference->getParallelTracks();
    int k = conference->getPapersInSession();
  //  int paperCounter = p*t*k;
    //Randomizer();
    double score = scoreOrganization();
    //cout << "score :" << score << endl;
    double max_score = score;
    double global_max = score;
    double new_score;
    int branching_factor = 10;//(int) pow(1.0 * paperCounter, 0.62) ;
    srand(time(NULL));
    clock_t start_time = clock();
    int a1,a2,b1,b2,c1,c2;
    bool updated = false;
    while(1){
      for(int j = 0; j < branching_factor; j++){
        int x1 = rand() % t;
        int x2 = rand() % t;
        int y1 = rand() % p;
        int y2 = rand() % p;
        int z1 = rand() % k;
        int z2 = rand() % k;
        SwapPapers(x1,y1,z1,x2,y2,z2);
        //new_score = updateScore(score,x1,y1,z1,x2,y2,z2);
        score = scoreOrganization();
        //cout << "score: " << score << " sc_old: " << score_1 << endl;
        if(score > max_score){
          max_score = score;
          a1 = x1;
          a2 = x2;
          b1 = y1;
          b2 = y2;
          c1 = z1;
          c2 = z2;
          updated = true;
        }
        SwapPapers(x1,y1,z1,x2,y2,z2);
      }
      if(updated){
        updated = !updated;
        //max_score = global_max;
        SwapPapers(a1,b1,c1,a2,b2,c2);
        //score = scoreOrganization();
        //cout << "new_score: " << new_score << " score:  " << score << endl;
      }
      if((int)(clock()-start_time)/CLOCKS_PER_SEC > 60){
          break;
        }
      }
}

void SessionOrganizer::readInInputFile ( string filename )
{
    vector<string> lines;
    string line;
    ifstream myfile ( filename.c_str () );
    if ( myfile.is_open ( ) )
    {
        while ( getline ( myfile, line ) )
        {
            //cout<<"Line read:"<<line<<endl;
            lines.push_back ( line );
        }
        myfile.close ( );
    }
    else
    {
        cout << "Unable to open input file";
        exit ( 0 );
    }

    if ( 6 > lines.size ( ) )
    {
        cout << "Not enough information given, check format of input file";
        exit ( 0 );
    }

    processingTimeInMinutes = atof ( lines[0].c_str () );
    papersInSession = atoi ( lines[1].c_str () );
    parallelTracks = atoi ( lines[2].c_str () );
    sessionsInTrack = atoi ( lines[3].c_str () );
    tradeoffCoefficient = atof ( lines[4].c_str () );

    int n = lines.size ( ) - 5;
    double ** tempDistanceMatrix = new double*[n];
    for ( int i = 0; i < n; ++i )
    {
        tempDistanceMatrix[i] = new double[n];
    }


    for ( int i = 0; i < n; i++ )
    {
        string tempLine = lines[ i + 5 ];
        string elements[n];
        splitString ( tempLine, " ", elements, n );

        for ( int j = 0; j < n; j++ )
        {
            tempDistanceMatrix[i][j] = atof ( elements[j].c_str () );
        }
    }
    distanceMatrix = tempDistanceMatrix;

    int numberOfPapers = n;
    int slots = parallelTracks * papersInSession*sessionsInTrack;
    if ( slots != numberOfPapers )
    {
        cout << "More papers than slots available! slots:" << slots << " num papers:" << numberOfPapers << endl;
        exit ( 0 );
    }
}

double** SessionOrganizer::getDistanceMatrix ( )
{
    return distanceMatrix;
}

void SessionOrganizer::printSessionOrganiser ( char * filename)
{
    conference->printConference ( filename);
}

double SessionOrganizer::scoreOrganization ( )
{
    // Sum of pairwise similarities per session.
    double score1 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession = tmpTrack.getSession ( j );
            for ( int k = 0; k < tmpSession.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession.getPaper ( k );
                for ( int l = k + 1; l < tmpSession.getNumberOfPapers ( ); l++ )
                {
                    int index2 = tmpSession.getPaper ( l );
                    score1 += 1 - distanceMatrix[index1][index2];
                }
            }
        }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack1 = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack1.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession1 = tmpTrack1.getSession ( j );
            for ( int k = 0; k < tmpSession1.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession1.getPaper ( k );

                // Get competing papers.
                for ( int l = i + 1; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    Session tmpSession2 = tmpTrack2.getSession ( j );
                    for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                    {
                        int index2 = tmpSession2.getPaper ( m );
                        score2 += distanceMatrix[index1][index2];
                    }
                }
            }
        }
    }
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}

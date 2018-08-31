/*
 * File:   SessionOrganizer.h
 * Author: Kapil Thakkar
 *
 */

#ifndef SESSIONORGANIZER_H
#define	SESSIONORGANIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <ctime>


#include "Conference.h"
#include "Track.h"
#include "Session.h"

using namespace std;


/**
 * SessionOrganizer reads in a similarity matrix of papers, and organizes them
 * into sessions and tracks.
 *
 * @author Kapil Thakkar
 *
 */
class SessionOrganizer {
private:
    double ** distanceMatrix;

    int parallelTracks ;
    int papersInSession ;
    int sessionsInTrack ;

    Conference *conference;

    double processingTimeInMinutes ;
    double tradeoffCoefficient ; // the tradeoff coefficient


public:
    SessionOrganizer();
    SessionOrganizer(string filename);


    /**
    * Swap two papers that are not in same session
    */
    void SwapPapers(int t1, int p1, int k1, int t2, int p2, int k2);


    /**
    * Randomize the given state to restart the algorithm
    */
    void Randomizer();

    /**
    * Effiecient value calculator without using scoreOrganization
    */
    double updateScore(double score, int x1, int y1, int z1, int x2, int y2, int z2);

    /**
     * Read in the number of parallel tracks, papers in session, sessions
     * in a track, and the similarity matrix from the specified filename.
     * @param filename is the name of the file containing the matrix.
     * @return the similarity matrix.
     */
    void readInInputFile(string filename);


    /**
     * Organize the papers according to some algorithm.
     */
    void organizePapers();


    /**
     * Get the distance matrix.
     * @return the distance matrix.
     */
    double** getDistanceMatrix();


    /**
     * Score the organization.
     * @return the score.
     */
    double scoreOrganization();


    void printSessionOrganiser(char *);
};

#endif	/* SESSIONORGANIZER_H */

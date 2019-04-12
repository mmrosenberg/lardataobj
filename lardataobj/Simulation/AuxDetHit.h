/*
 * File:   AuxDetHit.h
 * Author: wenzel
 *
 * Created on October 22, 2018, 2:35 PM
 */

#ifndef AUXDETHIT_H
#define AUXDETHIT_H

#include <vector>

namespace sim {

    class AuxDetHit {
    private:
        unsigned int ID; ///< Geant4 copy ID
        unsigned int trackID; ///< Geant4 supplied track ID
        float energyDeposited; ///< total energy deposited for this track ID and time
        float entryX; ///< Entry position X of particle
        float entryY; ///< Entry position Y of particle
        float entryZ; ///< Entry position Z of particle
        float entryT; ///< Entry time of particle
        float exitX; ///< Exit position X of particle
        float exitY; ///< Exit position Y of particle
        float exitZ; ///< Exit position Z of particle
        float exitT; ///< Exit time of particle
        float exitMomentumX; ///< Exit X-Momentum of particle
        float exitMomentumY; ///< Exit Y-Momentum of particle
        float exitMomentumZ; ///< Exit Z-Momentum of particle

    public:
        // Default constructor
        AuxDetHit() {}
        bool operator<(const AuxDetHit& other) const;
        bool operator==(const AuxDetHit& other) const;
        // Hide the following from Root

        AuxDetHit(unsigned int ID,
                unsigned int trackID,
                float energyDeposited,
                float entryX,
                float entryY,
                float entryZ,
                float entryT,
                float exitX,
                float exitY,
                float exitZ,
                float exitT,
                float exitMomentumX,
                float exitMomentumY,
                float exitMomentumZ) :
        ID(ID),
        trackID(trackID),
        energyDeposited(energyDeposited),
        entryX(entryX),
        entryY(entryY),
        entryZ(entryZ),
        entryT(entryT),
        exitX(exitX),
        exitY(exitY),
        exitZ(exitZ),
        exitT(exitT),
        exitMomentumX(exitMomentumX),
        exitMomentumY(exitMomentumY),
        exitMomentumZ(exitMomentumZ) {
        }

        void SetExitMomentumZ(float exitMomentumZ) {
            this->exitMomentumZ = exitMomentumZ;
        }

        float GetExitMomentumZ() const {
            return exitMomentumZ;
        }

        void SetExitMomentumY(float exitMomentumY) {
            this->exitMomentumY = exitMomentumY;
        }

        float GetExitMomentumY() const {
            return exitMomentumY;
        }

        void SetExitMomentumX(float exitMomentumX) {
            this->exitMomentumX = exitMomentumX;
        }

        float GetExitMomentumX() const {
            return exitMomentumX;
        }

        void SetExitT(float exitT) {
            this->exitT = exitT;
        }

        float GetExitT() const {
            return exitT;
        }

        void SetExitZ(float exitZ) {
            this->exitZ = exitZ;
        }

        float GetExitZ() const {
            return exitZ;
        }

        void SetExitY(float exitY) {
            this->exitY = exitY;
        }

        float GetExitY() const {
            return exitY;
        }

        void SetExitX(float exitX) {
            this->exitX = exitX;
        }

        float GetExitX() const {
            return exitX;
        }

        void SetEntryT(float entryT) {
            this->entryT = entryT;
        }

        float GetEntryT() const {
            return entryT;
        }

        void SetEntryZ(float entryZ) {
            this->entryZ = entryZ;
        }

        float GetEntryZ() const {
            return entryZ;
        }

        void SetEntryY(float entryY) {
            this->entryY = entryY;
        }

        float GetEntryY() const {
            return entryY;
        }

        void SetEntryX(float entryX) {
            this->entryX = entryX;
        }

        float GetEntryX() const {
            return entryX;
        }

        void SetEnergyDeposited(float energyDeposited) {
            this->energyDeposited = energyDeposited;
        }

        float GetEnergyDeposited() const {
            return energyDeposited;
        }

        void SetTrackID(unsigned int trackID) {
            this->trackID = trackID;
        }

        unsigned int GetTrackID() const {
            return trackID;
        }

        void SetID(unsigned int ID) {
            this->ID = ID;
        }

        unsigned int GetID() const {
            return ID;
        }

    };

    typedef std::vector<AuxDetHit> AuxDetHitCollection;
    inline bool sim::AuxDetHit::operator<(const AuxDetHit& other) const {    return trackID < other.trackID;}
    inline bool sim::AuxDetHit::operator==(const AuxDetHit& other) const {    return other.trackID == trackID;}
} // namespace sim
#endif /* AUXDETHIT_H */

// include/ActionInitialization.hh
#ifndef ACTIONINITIALIZATION_HH
#define ACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "BetaDecay.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization();
    virtual ~ActionInitialization();
    
    virtual void Build() const;
    virtual void BuildForMaster() const;
};

#endif // ACTIONINITIALIZATION_HH
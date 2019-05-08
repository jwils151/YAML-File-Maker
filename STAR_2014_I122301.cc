#include "Rivet/Tools/Cuts.hh"
#include "Rivet/HeavyIonAnalysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include <iostream>

namespace Rivet {


  /// @brief Add a short analysis description here
  
  class STAR_2014_I122301 : public HeavyIonAnalysis {
      
    private:
           
           
           /// Name Histograms
           
           //Figure 1
           
           Histo1DPtr _h_JHC_AuAu_Low_;
           Histo1DPtr _h_JHC_pp_Low_;
           
           //Figure 2
           
           Histo1DPtr _h_JHC_AuAu_High_;
           Histo1DPtr _h_JHC_pp_High_;
           
           //Figure 3
           
           Histo1DPtr _h_GW_pp_Low_;
           Histo1DPtr _h_GW_AuAu_Low_;
           Histo1DPtr _h_GW_pp_High_;
           Histo1DPtr _h_GW_AuAu_High_;
           
           //Figure 4
           
           Histo1DPtr _h_DAA_Low_;
           Histo1DPtr _h_DAA_High_;
           
  public:

    /// Constructor
    STAR_2014_I122301(): HeavyIonAnalysis("STAR_2014_I122301"){}

    /// Book histograms and initialise projections 
    
    void init() {
        
        /// Add Centrality Method
         addCentralityMethod(HeavyIonAnalysis::ImpactParameter, 50, "IPMethod");   
        
        ///Define Trigger Jets

        const FinalState fj1(FastJets::ANTIKT, 0.4); //Anti-Kt Jet Algorithm, R=0.4
        addProjection(fj1, "jetTrigger");
        
        
	
        /// Book Histograms
        
        //Figure 1
        _h_JHC_AuAu_Low_ = bookHisto1D("d01-x01-y01");
        _h_JHC_pp_Low_   = bookHisto1D("d02-x01-y01");
        
        //Figure 2
        _h_JHC_AuAu_High_ = bookHisto1D("d03-x01-y01");
        _h_JHC_pp_High_   = bookHisto1D("d04-x01-y01");
        
        //Figure 3
        _h_GW_pp_Low_    = bookHisto1D("d05-x01-y01");
        _h_GW_AuAu_Low_  = bookHisto1D("d06-x01-y01");
        _h_GW_pp_High_   = bookHisto1D("d07-x01-y01");
        _h_GW_AuAu_High_ = bookHisto1D("d08-x01-y01");
        
        //Figure 4
        _h_DAA_Low_      = bookHisto1D("d09-x01-y01");
        _h_DAA_High_     = bookHisto1D("d10-x01-y01");
    }
    /// Perform pre-event analysis
    void analyze(const Event& event) {
        
        
      
        //Fill Histograms of Figure 2a
        
        
        const ChargedFinalState & cfs = apply<ChargedFinalState>(event,"partAssoc");
        
        const Jets& jets = applyProjection<FastJets>(event,"jetTrigger").jetsByPt(); 
        
        //get particles from associated particle projection object
        Particles tracksAssoc = cfs.particlesByPt();
        
        const double c = centrality(event, "ImpactParameterMethod");
        
        double deltaPhi;
        foreach (const Jet& j, jets) {
            
                if ((j.pT() < 10) || (j.pT() > 15)) { 
                    
                    
                    foreach (const Particle& partAssoc, tracksAssoc) {
                        if (partAssoc.pt() < 0.5 || partAssoc.pt() < 1) //appropiact pt bin
                    
                    if((c > 0) || (c < 20)){  //should filter out pp colisions and only take Au+Au(0-20%)
                        
                        if (partAssoc.pt() > j.pt()) { 
                            //define delta phi
                            deltaPhi = partAssoc.phi() - j.phi();
                            
                        _h_JHC_AuAu_Low_->fill(deltaPhi, event.weight());
                        
                        }
                        }
                        }
                        }
                        }

    foreach (const Jet& j, jets) {
            
                if ((j.pT() < 10) || (j.pT() > 15)) { 
                    
                    
                    foreach (const Particle& partAssoc, tracksAssoc) {
                        if (partAssoc.pt() < 4 || partAssoc.pt() < 6) //appropiact pt bin
                    
                    if((c > 0) || (c < 20)){  //should filter out pp colisions
                        
                        if (partAssoc.pt() > j.pt()) { 
                        
                            deltaPhi = partAssoc.phi() - j.phi();
                            
                        _h_JHC_AuAu_High_->fill(deltaPhi, event.weight());
                        
                        }
                        }
                        }
                        }
                        }
                      
        //Fill Histograms of figure 2b
        foreach (const Jet& j, jets) {
            
                if ((j.pT() < 10) || (j.pT() > 15)) { 
                    
                    
                    foreach (const Particle& partAssoc, tracksAssoc) {
                        if (partAssoc.pt() < 0.5 || partAssoc.pt() < 1) //appropiact pt bin
                    
                        
                        if (partAssoc.pt() > j.pt()) { 
                        
                            deltaPhi = partAssoc.phi() - j.phi();
                            
                        _h_JHC_pp_Low_->fill(deltaPhi, event.weight());
                        
                        }
                        }
                        }
                        }
                        

                foreach (const Jet& j, jets) {
            
                if ((j.pT() < 10) || (j.pT() > 15)) { 
                    
                    
                    foreach (const Particle& partAssoc, tracksAssoc) {
                        if (partAssoc.pt() < 4 || partAssoc.pt() < 6) //appropiact pt bin
                    
                    
                        
                        if (partAssoc.pt() > j.pt()) { 
                        
                            deltaPhi = partAssoc.phi() - j.phi();
                            
                        _h_JHC_pp_High_->fill(deltaPhi, event.weight());
                        
                        }
                        }
                        }
                        }
                        
        //Fill HIstograms of figure 3
        //Fill Histograms of figure 4
                
            
        }
        
    
    /// Normalize Histograms
    void finalize(){
        
    }
    
	
};
DECLARE_RIVET_PLUGIN(STAR_2014_I122301);
}
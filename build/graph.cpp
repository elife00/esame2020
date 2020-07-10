#include <iostream>
{
    TGraph * graph_s = new TGraph ("trend.txt","%lg %lg %*lg %*lg");
    graph_s->SetTitle (
                       "epidemic's trend;"
                       "t (days);"
                       "population");
    TGraph * graph_i = new TGraph ("trend.txt","%lg %*lg %lg %*lg");
    TGraph * graph_r = new TGraph ("trend.txt","%lg %*lg %*lg %lg");

    // Cosmetics
    graph_s->SetLineColor(38);
    graph_s->SetLineWidth(2);
    graph_s->GetXaxis()->SetTitleOffset(1);
    graph_s->GetYaxis()->SetTitleOffset(1.3);
    graph_s->SetMinimum(0);
    
    graph_i->SetLineColor(46);
    graph_i->SetLineWidth(2);
    
    graph_r->SetLineColor(8);
    graph_r->SetLineWidth(2);
    
    graph_s->SetMarkerStyle(kFullDotLarge);
    graph_s->SetMarkerColor(38);
    graph_s->SetMarkerSize(0.3);
    graph_i->SetMarkerStyle(kFullDotLarge);
    graph_i->SetMarkerColor(46);
    graph_i->SetMarkerSize(0.3);
    graph_r->SetMarkerStyle(kFullDotLarge);
    graph_r->SetMarkerColor(8);
    graph_r->SetMarkerSize(0.3);
    

    // The canvas on which we'll draw the graph
    TCanvas *mycanvas = new TCanvas();
    mycanvas->SetGrid();
    // Draw the graph !
 
    gStyle->SetOptFit(111);
    graph_s->Draw();// assi, punto che ho scelto come marker, errori
    graph_i->Draw("CPSAME");
    graph_r->Draw("CPSAME");
    
    TLegend *leg=new TLegend(.7,.7,.9,.9,"Legend");
    
    leg->SetFillColor(0);
    leg->AddEntry(graph_s,"Susceptible");
    leg->AddEntry(graph_i,"Infected");
    leg->AddEntry(graph_r,"Recovered");
    leg->Draw("Same");
 
    mycanvas->Print("ModelloSir.gif");
}


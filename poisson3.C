#include <stdio.h>
#include <vector>



std::vector<float> n_0;


void file_read_to_vector(std::vector<float> *v, const char *location)
{
    FILE *fp;
    float tmp;

    fp = fopen(location, "r");
    
    if(fp == NULL)
    {
        fprintf(stderr, "Cannot open file!");
        exit(1);
    }

    while(fscanf(fp, "%f\n", &tmp) != EOF)
    {
        v->push_back(tmp);
    }

    fclose(fp);
}


void poisson3()
{
    file_read_to_vector(&n_0,  "data/n_0.csv");

    TCanvas *canvas = new TCanvas("c1", "Histogram", 500, 500);
    canvas->SetLeftMargin(0.15);
    TH1F *h1 = new TH1F("h3", "Distribution of Time Intervals Between Adjacent Pulses (n=0)", 10,0,8);

    for(int i=0; i<n_0.size(); i++)
    {
        h1->Fill(n_0[i]);
    }
    h1->Scale(1.0f/h1->Integral());

    //gStyle->SetOptFit(0);
    //gStyle->SetOptStat(0);


    h1->SetMaximum(0.52);
    h1->GetXaxis()->SetTitle("Time interval [s]");
    h1->GetYaxis()->SetTitle("Frequency");


   
    TF1 *poiss = new TF1("f2",  "  [0] * exp(-[1] * x) * [1]");
    poiss->SetParameters(1,1,1);
    poiss->SetLineColor(kGreen);
    



    h1->SetLineColor(1);
    h1->Fit(poiss, "+");
    h1->SetFillColor(41);
    h1->DrawClone("HIST");
    h1->SetLineColor(2);
    h1->DrawClone("E1 SAME");

     gPad->Update(); //to force the creation of "stats"

    auto p_chi =  poiss->GetChisquare();
    auto p_n = poiss->GetNDF();


    std::cout << "\n----Poisson----\n"<< "chi^2:" << p_chi << "\tn:" << p_n << "\t\tchi_per_n:" << (float)(p_chi/p_n) << "\n";

    TLegend legend;
    legend.AddEntry(h1, "Error Bars", "lep");
    legend.AddEntry(poiss, "Poisson Fit", "l");
    legend.DrawClone("same");

}


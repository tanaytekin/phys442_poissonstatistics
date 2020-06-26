#include <stdio.h>
#include <vector>



std::vector<int> d_1;


void file_read_to_vector(std::vector<int> *v, const char *location)
{
    FILE *fp;
    int tmp;

    fp = fopen(location, "r");
    
    if(fp == NULL)
    {
        fprintf(stderr, "Cannot open file!");
        exit(1);
    }

    while(fscanf(fp, "%d\n", &tmp) != EOF)
    {
        v->push_back(tmp);
    }

    fclose(fp);
}






void poisson2()
{
    file_read_to_vector(&d_1,  "data/1s.csv");

    TCanvas *canvas = new TCanvas("c1", "Histogram", 500, 500);
    canvas->SetLeftMargin(0.15);
    TH1F *h1 = new TH1F("h2", "Distribution of Decays in 1s Intervals", 10,0,25);

    for(int i=0; i<d_1.size(); i++)
    {
        h1->Fill(d_1[i]);
    }
    h1->Scale(1.0f/h1->Integral());

    //gStyle->SetOptFit(0);
    //gStyle->SetOptStat(0);


    h1->SetMaximum(0.4);
    h1->GetXaxis()->SetTitle("Number of Decays");
    h1->GetYaxis()->SetTitle("Frequency");


    TF1 *gaus = new TF1("f1","gausn");
    gaus->SetLineColor(kBlue);
    h1->Fit(gaus, "");
   
    TF1 *poiss = new TF1("f2",  "[0] * TMath::Poisson(x,[1])");
    poiss->SetParameters(60,60,60);
    poiss->SetLineColor(kGreen);
    



    h1->SetLineColor(1);
    h1->Fit(poiss, "+");
    h1->SetFillColor(41);
    h1->DrawClone("HIST");
    h1->SetLineColor(2);
    h1->DrawClone("E1 SAME");

     gPad->Update(); //to force the creation of "stats"

    auto g_chi =  gaus->GetChisquare();
    auto g_n = gaus->GetNDF();
    auto p_chi =  poiss->GetChisquare();
    auto p_n = poiss->GetNDF();

    std::cout << "\n----Gaussian----\n"<< "chi^2:" << g_chi << "\tn:" << g_n << "\t\tchi_per_n:" << (float)(g_chi/g_n) << "\n";

    std::cout << "\n----Poisson----\n"<< "chi^2:" << p_chi << "\tn:" << p_n << "\t\tchi_per_n:" << (float)(p_chi/p_n) << "\n";

    TLegend legend;
    legend.AddEntry(h1, "Error Bars", "lep");
    legend.AddEntry(gaus, "Gaussian Fit", "l");
    legend.AddEntry(poiss, "Poisson Fit", "l");
    legend.DrawClone("same");

}


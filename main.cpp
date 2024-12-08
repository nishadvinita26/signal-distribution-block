#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

// Amplifier Class
class Amplifier {
public:
    double gain; // Amplifier gain in dB
    Amplifier(double g) : gain(g) {}
    double amplify(double inputPower) {
        return inputPower + gain; // Output Power in dBm
    }
};

// Attenuator Class
class Attenuator {
public:
    double attenuation; // Attenuation in dB
    Attenuator(double a) : attenuation(a) {}
    double attenuate(double inputPower) {
        return inputPower - attenuation; // Output Power in dBm
    }
};

// Switch Class
class Switch {
public:
    bool state; // ON = true, OFF = false
    Switch(bool s) : state(s) {}
    double passSignal(double inputPower) {
        return state ? inputPower : -1e6; // Pass signal or block it (-1e6 represents OFF state)
    }
};

// Power Divider Class
class PowerDivider {
public:
    int numOutputs;          // Number of outputs
    double dividerLoss = 1;  // Loss due to division (3 dB per branch)

    PowerDivider(int n) : numOutputs(n) {}

    vector<double> dividePower(double inputPower) {
        vector<double> outputs(numOutputs, inputPower - dividerLoss);
        return outputs; // Outputs with 3 dB attenuation for each branch
    }
};

// Circuit Simulation
void simulateCircuit() {
    // Input parameters
    double inputPower = 30.0; // Input power in dBm
    Amplifier ampA(19);        // Amplifier A gain at 1 GHz cost 20$
    // Amplifier ampB(19);        // Amplifier B gain at 1 GHz
    Amplifier ampE(17.5);       // Amplifier E gain at 1GHz cost 17$
    Attenuator attF(-6.6);        // Attenuator F cost 16$
    // Attenuator attB(-6.6);        // Attenuator B
    // Attenuator attD(-6.6);        // Attenuator D
    Switch swA(true);          // Switch C (ON)  Cost 13$
    Switch swB(true);          // Switch C (ON)  Cost 13$
    PowerDivider powerDivider(2); // Power divider with 2 outputs

    cout << "Simulating signal path..." << endl;

    // Step 1: Apply Power Divider
    vector<double> dividedOutputs = powerDivider.dividePower(inputPower);
    cout << "Power Divider Outputs:" << endl;
    for (size_t i = 0; i < dividedOutputs.size(); ++i) {
        cout << "  Output " << i + 1 << ": " << dividedOutputs[i] << " dBm" << endl;
    }

    // Branch 1 Simulation
    cout << "\n--- Branch 1 Simulation ---" << endl;
    double branch1Power = dividedOutputs[0];
    branch1Power = attF.attenuate(branch1Power);
    cout << "After Attenuator D: " << branch1Power << " dBm" << endl;
    branch1Power = ampA.amplify(branch1Power);
    cout << "After Amplifier A: " << branch1Power << " dBm" << endl;
    branch1Power = attF.attenuate(branch1Power);
    cout << "After Attenuator A: " << branch1Power << " dBm" << endl;
    branch1Power = swA.passSignal(branch1Power);
    if (branch1Power < -1e5) {
        cout << "Signal blocked by Switch A" << endl;
    } else {
        cout << "After Switch A: " << branch1Power << " dBm" << endl;
    }
    
    branch1Power = attF.attenuate(branch1Power);
    cout << "After Attenuator B: " << branch1Power << " dBm" << endl;
    
    branch1Power = ampE.amplify(branch1Power);
    cout << "After Amplifier D: " << branch1Power << " dBm" << endl;

    // Branch 2 Simulation
    cout << "\n--- Branch 2 Simulation ---" << endl;
    double branch2Power = dividedOutputs[1];
    branch2Power = attF.attenuate(branch2Power);
    cout << "After Attenuator D: " << branch2Power << " dBm" << endl;
    branch2Power = ampA.amplify(branch2Power);
    cout << "After Amplifier A: " << branch2Power << " dBm" << endl;
    branch2Power = attF.attenuate(branch2Power);
    cout << "After Attenuator A: " << branch2Power << " dBm" << endl;
    
    branch2Power = swB.passSignal(branch2Power);
    if (branch2Power < -1e5) {
        cout << "Signal blocked by Switch A" << endl;
    } else {
        cout << "After Switch A: " << branch2Power << " dBm" << endl;
    }
    
    
    branch2Power = attF.attenuate(branch2Power);
    cout << "After Attenuator B: " << branch2Power << " dBm" << endl;

    branch2Power = ampE.amplify(branch2Power);
    cout << "After Amplifier D: " << branch2Power << " dBm" << endl;
    
    // Final Output
    cout << "\nFinal Outputs:" << endl;
    cout << "  Branch 1 Output: " << branch1Power << " dBm" << endl;
    cout << "  Branch 2 Output: " << branch2Power << " dBm" << endl;
}

int main() {
    simulateCircuit();
    return 0;
}

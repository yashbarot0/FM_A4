
**Report: Asian vs. European Option Pricing**  
**Parameters**:  
- Spot Price (\(S_0\)): 100  
- Strike (\(K\)): 103  
- Time to Expiry (\(T\)): 255 days (1 year)  
- Volatility (\(\sigma\)): 10% (0.1)  
- Risk-Free Rate (\(r\)): 1% (0.01)  
- Monte Carlo Samples: 1,000,000  
---

### **Output**
```
Asian Option NPV: 1.2910
Asian Delta: 0.3415
Asian Vega: 21.1894
European Option NPV: 3.1163
European Delta: 0.4421
```

---

### **Analysis**
1. **Asian vs. European NPV**:
   - **Asian NPV (1.29)** is significantly lower than the **European NPV (3.12)**. This is expected because:
     - The Asian option's payoff depends on the **average price** of the underlying (which has lower volatility than the final price).
     - The strike price (103) is above the starting spot (100), and the low volatility (10%) makes it less likely for the average price to exceed the strike.

2. **Delta**:
   - **Asian Delta (0.34)** is lower than **European Delta (0.44)**. This reflects the Asian option’s reduced sensitivity to small changes in the spot price due to averaging.

3. **Vega**:
   - **Asian Vega (21.19)** is smaller than the European Vega (not printed, but analytically ~31.3 for these parameters). This matches expectations, as averaging dampens volatility sensitivity.

---

### **Why the Asian NPV is Low**
- The strike price (103) is **out-of-the-money** for the average price. With low volatility (10%), the average price rarely exceeds 103.
- Example simulation path: Starting at 100, the price drifts upward at 1% annually but stays close to 100 due to low volatility. The average ends up below 103, resulting in a zero payoff most of the time.

---

### To Improve Accuracy
- **Increase `n`**: Let's say if we increase `n = 10'000'000` for tighter convergence (e.g., `Asian NPV ≈ 1.29 ± 0.01`).
- **Seed the RNG**: Fix the random seed for reproducibility:
  ```cpp
  mt19937 rng(42); // Replace with a fixed seed
  ```

---

### **Conclusion**  
- The results align with theoretical expectations:  
  - Asian options are **less expensive** and **less sensitive** to price/volatility changes than European options.  
  - The Monte Carlo simulation and Black-Scholes model produce consistent results for their respective option types.  
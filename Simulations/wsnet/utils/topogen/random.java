
import java.security.*;
import java.util.*;

public class random {

	Random r;
        static final long SEED=System.currentTimeMillis();
	
       public random() {
		r=new Random(SEED);
	}
	
	public int nextRandom(int a, int b) {
		if (a==b) return a;
		else return (a+r.nextInt(b-a+1));
	}
	
	public double nextRandom() {
		return r.nextDouble();
	}
	
	
	public double nextPoisson(int intensite)	{
	  double y, resultat=0;
	  int i, nonTrouve;
	  int max, min;
	  min = (int)(intensite - 2 * Math.sqrt(intensite));
	  max = (int)(intensite + 2 * Math.sqrt(intensite));
	  nonTrouve = 1;
	  y = nextRandom() * somme(intensite, max);
	  i =min;
	  while((nonTrouve == 1)&&(i<max +1)) {
	      if (y < somme(intensite,i)) {
	          resultat = i;
	          nonTrouve = 0;
	        }
	      i++;
	    }
	  return (resultat);
	}

	double fonctionGaussienne(int intensite, int val) {
	  return ( (1/(2*Math.PI*Math.sqrt((double) intensite))) * Math.exp(-(double)Math.pow(val-intensite,2)/(2.0*intensite)) );
	}
	
	double somme(int intensite, int max) {
	  double somme=0;
	  int i = (int)(intensite - 2*Math.sqrt(intensite));
	  while (i < max+1) {
	      somme += fonctionGaussienne(intensite,i);
	      i++;
      }
	  return somme;
	}
	
	double fact(int n) {
	  if (n==0) return 1;
	  else
	    return n*fact(n-1);
	}


}

class node_set {

    private int   id;
    private float x;
    private float y;

    public node_set(int id, double x, double y){
	this.id = id;
	this.x  = (float) x;
	this.y  = (float) y;
    }

    void setId(int id){
	this.id=id;
    }

    int getId(){
	return id;
    }

    double getX(){
	return x;
    }

    double getY(){
	return y;
    }
}

// I’d want a block class with at least these fields and methods.
public class Block {
    private String name;
    private int offset;
    private int size;

    public Block(String name, int size, int offset){
        this.offset = offset;
        this.name = name;
        this.size = size;
    }

    public String toString(){  // highly recommended
        return this.name;
    }

    public boolean is_adjacent(Block other) {
        if (this.getOffset() < other.getOffset())
            return this.getOffset() + this.getSize() == other.getOffset();
        else
            return other.getOffset() + other.getSize() == this.getOffset();
    }

    public int getOffset() {
        return offset;
    }

    public void setOffset(int offset) {this.offset = offset;}

    public int getSize() {
        return size;
    }

    public void setSize(int size){this.size = size;}

    public void setName(String name){   this.name = name; }
}
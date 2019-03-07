import java.util.Comparator;

/**
 * Created by mltaskova on 3/6/19.
 */
public class BySize implements Comparator<Block> {
    @Override
    public int compare(Block o1, Block o2) {
        return Integer.compare(o1.getSize(), o2.getSize());
    }
}

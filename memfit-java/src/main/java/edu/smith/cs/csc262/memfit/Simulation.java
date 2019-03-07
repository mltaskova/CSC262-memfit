//package edu.smith.cs.csc262.memfit;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

public class Simulation {

  private String algorithm;
  private List<Block> free_list;
  private List<Block> used_list;
  private Block last;
  private int totalSize;
  private int allocFail;
  private int freeFail;

  public Simulation(){
      this.free_list = new LinkedList<>();
      this.used_list = new LinkedList<>();
      this.totalSize = 0;
      this.allocFail = 0;
      this.freeFail = 0;
      this.last = new Block("", 0, 0);
  }

  public void commandProcess(String[] args){
      if (args[0].equals("pool")){
          this.algorithm = args[1];
          this.totalSize += Integer.valueOf(args[2]);
          System.out.println("pool of size " + args[2] + " allocator " + args[1]);
          //something more special
          this.free_list.add(new Block("pool", Integer.valueOf(args[2]), 0));
      }
      else if (args[0].equals("alloc")){
          if (this.algorithm.equals("first"))
              alloc_first(args[1], Integer.valueOf(args[2]));
          else if (this.algorithm.equals("worst"))
              alloc_worst_fit(args[1], Integer.valueOf(args[2]));
          else if (this.algorithm.equals("best"))
              alloc_best_fit(args[1], Integer.valueOf(args[2]));
          else if (this.algorithm.equals("random"))
              alloc_random(args[1], Integer.valueOf(args[2]));
          else if (this.algorithm.equals("next"))
              alloc_next_fit(args[1], Integer.valueOf(args[2]));
      }
      else{
          free(args[1]);
          compactFreeList();
      }
  }

  // Strongly recommend you start with printing out the pieces.
  private void print(){
      double ratio = 0.00;
      Collections.sort(this.free_list, new ByOffset());
      for (Block b:this.free_list){
          ratio += (double) b.getSize();
          System.out.println(b.toString() + " offset: "+ b.getOffset()+ " size: " + b.getSize());
      }
      System.out.println("free: " + (ratio/(double) this.totalSize)*100.00 + "%");
      ratio = 0.00;
      Collections.sort(this.used_list, new ByOffset());
      for (Block b:this.used_list){
          ratio += (double) b.getSize();
          System.out.println(b.toString() + " offset: "+ b.getOffset()+ " size: " + b.getSize());
      }
      System.out.println("used: " + (ratio/(double) this.totalSize)*100.00 + "%");
      System.out.println("alloc failed " + this.allocFail + " times");
      System.out.println("free failed "+ this.freeFail + " times");
  }

  private void alloc(String name, int size) {
      for (Block b : this.free_list){
          if (b.getSize() >= size){
              splitBlock(b, name, size);
              return;
          }
      }
      this.allocFail++;
  }

  private void alloc_next_fit(String name, int size){
      Collections.sort(this.free_list, new ByOffset());
      if (this.last.toString().equals("") || this.last.getOffset()+this.last.getSize() >= this.totalSize) {
          alloc(name, size);
          return;
      }
      for (Block b:free_list){
          if (b.getOffset() > this.last.getOffset() && b.getSize() >= size) {
              splitBlock(b, name, size);
              return;
          }
      }
  }

  private void alloc_best_fit(String name, int size){
      Collections.sort(this.free_list, new BySize());
      alloc(name, size);
  }

  private void alloc_worst_fit(String name, int size){
      Collections.sort(this.free_list, Collections.reverseOrder(new BySize()));
      alloc(name, size);
  }

  private void alloc_random(String name, int size){
      Collections.shuffle(this.free_list);
      alloc(name, size);
  }

  private void alloc_first(String name, int size){
      Collections.sort(this.free_list, new ByOffset());
      alloc(name, size);
  }

  private void free(String name){
      for (Block b : this.used_list){
          if (b.toString().equals(name)){
              this.free_list.add(b);
              this.used_list.remove(b);
              return;
          }
      }
      this.freeFail++;
  }

  private void splitBlock(Block b, String newName, int size) {
    if (b.getSize() == size){
        this.free_list.remove(b);
        b.setName(newName);
        this.used_list.add(b);
    }
    else{
        this.used_list.add(new Block(newName, size, b.getOffset()));
        b.setSize(b.getSize() - size);
        b.setOffset(b.getOffset() + size);
    }
  }
  private void compactFreeList() {
      Collections.sort(this.free_list, new ByOffset());
      List<Block> compactList = new LinkedList<>();
      Block temp = new Block("temp", 0, 0);
      for (Block b : this.free_list){
          if (temp.is_adjacent(b))
              temp.setSize(temp.getSize() + b.getSize());
          else{
              if (temp.getSize() > 0)
                  compactList.add(temp);
              temp = b;
          }
      }
      if (temp.getSize() > 0)
          compactList.add(temp);
      this.free_list = new LinkedList<>(compactList);
  }

  public static void main(String[] args) throws IOException {
      Simulation mySim = new Simulation();
    for (String line: Files.readAllLines(new File(args[0]).toPath())){
      mySim.commandProcess(line.split(" "));
    }
    mySim.print();
  }
}

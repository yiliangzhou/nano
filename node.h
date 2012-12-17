#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

struct node {
  public: 
    // default constructor
    node () { flags = INIT_FLAGS; } 

    // copy constructor
    node (node & rhs) { this->flags = rhs.flags; }
    node& operator=(node &rhs) { this->flags = rhs.flags; return *this; }
    // flag current node to reflect eaten.
    void munch () { flags |= MUNCHED; }

    // initialization methods
    void set_valid () { flags |= IS_VALID; }
    void connect_to_left () { flags |= HAS_LEFT_EDGE; }
    void connect_to_right () { flags |= HAS_RIGHT_EDGE; }
    void connect_to_up () { flags |= HAS_UP_EDGE; }
    void connect_to_down () { flags |= HAS_DOWN_EDGE; } 
    void connect_to (int x_dir, int y_dir) {
      if (x_dir < 0 && y_dir == 0) { flags |= HAS_RIGHT_EDGE; return; }
      if (x_dir > 0 && y_dir == 0) { flags |= HAS_LEFT_EDGE; return; }
      if (x_dir == 0 && y_dir < 0) { flags |= HAS_DOWN_EDGE; return; }
      if (x_dir == 0 && y_dir > 0) { flags |= HAS_UP_EDGE; return; }
    }
    // test if this is a valid node.
    bool is_valid () { return (flags & IS_VALID) != INIT_FLAGS; }

    // testing method, valid only when is_valid() return true.
    bool is_munched () { return (flags & MUNCHED) != INIT_FLAGS; }
    bool has_left_edge () { return (flags & HAS_LEFT_EDGE) != INIT_FLAGS; }
    bool has_right_edge () { return (flags & HAS_RIGHT_EDGE) != INIT_FLAGS; }
    bool has_up_edge () { return (flags & HAS_UP_EDGE) != INIT_FLAGS; } 
    bool has_down_edge () { return (flags & HAS_DOWN_EDGE) != INIT_FLAGS; }
  private:
    static const unsigned char HAS_RIGHT_EDGE = 0x01;
    static const unsigned char HAS_UP_EDGE = 0x02;
    static const unsigned char HAS_LEFT_EDGE = 0x04;
    static const unsigned char HAS_DOWN_EDGE = 0x08;
    static const unsigned char MUNCHED = 0x10;
    static const unsigned char IS_VALID = 0x20;
    static const unsigned char INIT_FLAGS = 0x00;

    // Flags to indicate current node's status.
    unsigned char flags;
};

#endif

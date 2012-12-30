#ifndef __NODE_H_INCLUDED__
#define __NODE_H_INCLUDED__

struct node {
  public: 
    // default constructor
    node () { flags = INIT_FLAGS; } 

    // copy constructor
    node (node & rhs) { 
      this->flags = rhs.flags; 
      this->node_id = rhs.node_id;
      this->player = rhs.player; 
      this->l_id = rhs.l_id;
      this->r_id = rhs.r_id;
      this->u_id = rhs.u_id;
      this->d_id = rhs.d_id;
    }
    node& operator=(node &rhs) { 
      this->flags = rhs.flags; 
      this->node_id = rhs.node_id;
      this->player = rhs.player; 
      this->l_id = rhs.l_id;
      this->r_id = rhs.r_id;
      this->u_id = rhs.u_id;
      this->d_id = rhs.d_id;  
      return *this; 
    }
    
    // flag node to reflect eaten.
    void munch () { flags |= MUNCHED; }
    void munch (int player) { 
        flags |= MUNCHED; 
        this->player = player; 
    }

    // initialization methods
    void set_valid (int node_id) { flags |= IS_VALID; this->node_id = node_id; }
    void connect_to (int x_dir, int y_dir, int id) {
      if (x_dir < 0 && y_dir == 0) { flags |= HAS_RIGHT_EDGE; r_id = id; return; }
      if (x_dir > 0 && y_dir == 0) { flags |= HAS_LEFT_EDGE; l_id = id; return; }
      if (x_dir == 0 && y_dir < 0) { flags |= HAS_DOWN_EDGE; d_id = id; return; }
      if (x_dir == 0 && y_dir > 0) { flags |= HAS_UP_EDGE; u_id = id; return; }
    }
    // test if this is a valid node.
    bool is_valid () { return (flags & IS_VALID) != INIT_FLAGS; }
    
    int get_node_id () { return node_id; }
    int get_l_id () { return l_id; }
    int get_r_id () { return r_id; }
    int get_u_id () { return u_id; }
    int get_d_id () { return d_id; }

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
    int node_id;
    // Munched by 1: red, 2: blue
    int player;
    int l_id;
    int r_id;
    int u_id;
    int d_id;
};

#endif

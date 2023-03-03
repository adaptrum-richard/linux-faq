#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/rbtree.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/module.h>
struct mytype{
    struct rb_node node;
    int key;
};

//查找
static struct mytype * my_search(struct rb_root *root, int new)
{
    struct rb_node *rnode = root->rb_node;
    while(rnode){
        struct mytype *data = container_of(rnode, struct mytype, node);
        if(data->key > new)
            rnode = rnode->rb_left;
        else if(data->key < new)
            rnode = rnode->rb_right;
        else 
            return data;
    }
    return NULL;
}

//增加
static int my_insert(struct rb_root *root, struct mytype *data)
{
    struct rb_node **new = &(root->rb_node), *parent = NULL;
    while(*new){
        struct mytype *this = container_of(*new, struct mytype, node);
        parent = *new;
        if(this->key > data->key)
            new = &((*new)->rb_left);
        else if(this->key < data->key)
            new = &((*new)->rb_right);
        else 
            return -1;
    }
    rb_link_node(&data->node, parent, new);
    rb_insert_color(&data->node, root);
    return 0;
}

//删除
static void my_delete(struct rb_root *root, int key)
{
    struct mytype *data;
    struct rb_node *node;
    for(node = rb_first(root); node; node = rb_next(node)){
        data = rb_entry(node, struct mytype, node);
        if(data->key == key){
            rb_erase(&data->node, root);
            kfree(data);
        }
    }    
}

//修改
static int my_modify(struct rb_root *root, int from, int to)
{
    struct mytype *data;
    struct rb_node *node;
    for(node = rb_first(root); node; node = rb_next(node)){
        data = rb_entry(node, struct mytype, node);
        if(data->key == from){
            rb_erase(&data->node, root);
            data->key = to;
            my_insert(root, data);
            return 0;
        }
    }
    return -1;
}

struct rb_root mytree = RB_ROOT;

static int __init my_init(void)
{
    int i;
    struct mytype *data;
    struct rb_node *node;
    int delete_key = 14, search_key = 16;
    //增加
    for(i = 0; i < 20; i+=2){
        data = kmalloc(sizeof(struct mytype), GFP_KERNEL);
        data->key = i;
        my_insert(&mytree, data);
    }
    //遍历
    for(node = rb_first(&mytree); node; node = rb_next(node))
        printk("key = %d\n", rb_entry(node, struct mytype, node)->key);

    //删除
    my_delete(&mytree, delete_key);

    printk("delete key : %d\n", delete_key);

    //查找
    data = my_search(&mytree, search_key);
    printk("search key: %d\n", data->key);

    //遍历
    for(node = rb_first(&mytree); node; node = rb_next(node))
        printk("key = %d\n", rb_entry(node, struct mytype, node)->key);

    //修改
    my_modify(&mytree, 10 , 1);
    printk("modify key: 10 to 1 \n");

    //遍历
    for(node = rb_first(&mytree); node; node = rb_next(node))
        printk("key = %d\n", rb_entry(node, struct mytype, node)->key);
    
    return 0;
}

static void __exit my_exit(void)
{
    struct mytype *data;
    struct rb_node *node;
    for(node = rb_first(&mytree); node; node = rb_next(node)){
        data = rb_entry(node, struct mytype, node);
        if(data){
            rb_erase(&data->node, &mytree);
            kfree(data);
        }
    }
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);

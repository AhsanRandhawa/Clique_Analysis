import random

def get_random_adjacent_node(cur_node, size_graph: int) -> int:
    index = random.randrange(size_graph - 1)
    if cur_node <= index:
        index += 1
    return index


# returns (number of rounds until node 0 finds entire graph, number of rounds until all nodes find entire graph)
def experiment_clique_random(size_graph: int) -> tuple[int, int]:
    if size_graph == 1:
        return 0, 0

    # the nodes that node i know about by the last iteration
    nodes_found_prev = [{i} for i in range(size_graph)]

    # the nodes that node i know about by this iteration
    nodes_found_next = [{i} for i in range(size_graph)]

    cur_round = 0
    rounds_for_1st_node = 0

    # The set of the nodes that do not know the whole graph yet
    nodes_to_check = set([i for i in range(size_graph)])

    while nodes_to_check:
        cur_round += 1

        # The set of nodes that found out about the entire graph in this round
        to_be_deleted_from_search = set()

        for node in nodes_to_check:
            other_node = get_random_adjacent_node(node, size_graph)

            # update this node by the amount
            nodes_found_next[node].update(nodes_found_prev[other_node])
            if len(nodes_found_next[node]) == size_graph:
                # This node knows the entire graph
                to_be_deleted_from_search.add(node)
                if node == 0:
                    rounds_for_1st_node = cur_round
        for node in nodes_to_check:
            nodes_found_prev[node].update(nodes_found_next[node])
        nodes_to_check.difference_update(to_be_deleted_from_search)

    return rounds_for_1st_node, cur_round


# runs iterations number of experiments for size_graph cliques and prints mean rounds for 1st node to find entire graph
# and the mean rounds for all nodes to find the entire graph
def run_experiments(iterations: int, size_graph: int) -> None:
    mean_rounds_for_1st_node = 0
    mean_rounds_for_all_nodes = 0
    for _ in range(iterations):
        r_1, r_all = experiment_clique_random(size_graph)
        mean_rounds_for_1st_node += r_1
        mean_rounds_for_all_nodes += r_all

    mean_rounds_for_all_nodes /= iterations
    mean_rounds_for_1st_node /= iterations
    print("Mean Rounds for 1st node to know the entire graph in clique of order", size_graph, "was",
          mean_rounds_for_1st_node)
    print("Mean Rounds for all nodes to know the entire graph in clique of order", size_graph, "was",
          mean_rounds_for_all_nodes)


if __name__ == '__main__':
    run_experiments(10, 2000)


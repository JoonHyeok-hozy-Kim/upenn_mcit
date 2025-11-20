import matplotlib.pyplot as plt
import numpy as np

def draw_discrete_gradient():
    # Create figure
    fig, ax = plt.subplots(figsize=(8, 5))

    # Define states and probabilities
    states = ['State x', 'State y']
    x_pos = [1, 3]
    probs = [2.0, 5.0]  # p(x)=2, p(y)=5

    # Plot bars for probabilities
    bars = ax.bar(x_pos, probs, width=0.6, color=['#a8d0e6', '#f8b195'], alpha=0.8, edgecolor='black')

    # Add transition arrow (Edge xy)
    ax.annotate('', xy=(3, 5), xytext=(1, 2),
                arrowprops=dict(facecolor='black', shrink=0.05, width=1.5, headwidth=8))

    # Label the gradient (Difference)
    # Draw a line showing the difference level
    ax.plot([3, 3], [2, 5], color='red', linestyle='--', linewidth=2)
    ax.plot([1, 3], [2, 2], color='gray', linestyle=':', linewidth=1.5)

    # Add text labels
    ax.text(1, 2.2, '$p(x)$', ha='center', fontsize=12, fontweight='bold')
    ax.text(3, 5.2, '$p(y)$', ha='center', fontsize=12, fontweight='bold')
    
    # Label for the arrow
    ax.text(2, 3.8, 'Transition $xy$', ha='center', rotation=35, fontsize=11)

    # Label for the gradient
    ax.text(3.1, 3.5, r'$\nabla p(xy) = p(y) - p(x)$', ha='left', color='red', fontsize=12, fontweight='bold')

    # Set chart limits and labels
    ax.set_ylim(0, 6)
    ax.set_xlim(0, 4)
    ax.set_xticks(x_pos)
    ax.set_xticklabels(states, fontsize=12)
    ax.set_ylabel('Probability Mass / Function Value', fontsize=12)
    ax.set_title('Visualizing Discrete Gradient $\\nabla p(xy)$', fontsize=14)
    
    # Remove top and right spines
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)

    # Save the plot
    plt.savefig('discrete_gradient_visual.png')

draw_discrete_gradient()